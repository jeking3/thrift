/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <amqp_tcp_socket.h>
#include <sstream>
#include <string.h>
#include <thrift/transport/TRabbitMQSession.h>

namespace apache {
namespace thrift {
namespace transport {

TRabbitMQSession::TRabbitMQSession(const TRabbitMQSessionOptions& opts)
  : m_opts(opts), m_consumer(false), m_delivery_tag(0), m_rpos(0)
{
}

TRabbitMQSession::~TRabbitMQSession()
{
}

bool TRabbitMQSession::isOpen()
{
    return static_cast<bool>(m_conn);
}

bool TRabbitMQSession::peek()
{
    return isOpen();
}

void TRabbitMQSession::open()
{
    // open() is not idempotent
    if (isOpen())
    {
        throw TTransportException(TTransportException::INTERNAL_ERROR, "connection is already open");
    }

    // remember if this is a consumer or a producer
    m_consumer = !m_opts.queue.name.empty();

    // at the end, if everything works, conn gets moved into m_conn
    stdcxx::shared_ptr<amqp_connection_state_t_> conn(amqp_new_connection(), amqp_destroy_connection);

    // TODO: add ssl support
    if (m_opts.tls.enabled)
    {
        throw TTransportException(TTransportException::INTERNAL_ERROR, "ssl/tls support is not implemented");
    }

    // the socket pointer is managed by the connection - we do not free it here
    amqp_socket_t *socket = checkNotNull(
       "amqp_tcp_socket_new",
        amqp_tcp_socket_new(conn.get()),
        conn.get(),
        TTransportException::INTERNAL_ERROR);
        
    // open the socket
    checkResult(
       "amqp_socket_open",
        amqp_socket_open(socket, m_opts.conn.host.c_str(), m_opts.conn.port),
        TTransportException::NOT_OPEN);
    
    // login
    (void) checkRpcReply(
       "amqp_login",
        amqp_login(
            conn.get(),
            m_opts.conn.vhost.c_str(),
            m_opts.conn.channel,
            m_opts.conn.frame_max,
            m_opts.conn.heartbeat,
            AMQP_SASL_METHOD_PLAIN,
            m_opts.conn.user.c_str(),
            m_opts.conn.pass.c_str()),
        TTransportException::NOT_OPEN);

    // open a channel
    (void) checkNotNull(
       "amqp_channel_open",
        amqp_channel_open(conn.get(), m_opts.conn.channel),
        conn.get(),
        TTransportException::INTERNAL_ERROR);

    // confirm select
    (void) checkNotNull(
       "amqp_confirm_select",
        amqp_confirm_select(conn.get(), m_opts.conn.channel),
        conn.get(),
        TTransportException::INTERNAL_ERROR);

    if (!m_opts.exch.name.empty())
    {
        // create or connect to the named exchange
        (void) checkNotNull(
           "amqp_exchange_declare",
            amqp_exchange_declare(
                conn.get(),
                m_opts.conn.channel,
                amqp_cstring_bytes(m_opts.exch.name.c_str()),
                amqp_cstring_bytes(m_opts.exch.type.c_str()),
                m_opts.exch.passive,
                m_opts.exch.durable,
                m_opts.exch.auto_delete,
                false, /* internal */
                amqp_empty_table),
            conn.get(),
            TTransportException::INTERNAL_ERROR);
    }

    // create or connect to a queue
    amqp_queue_declare_ok_t *qdo = checkNotNull(
       "amqp_queue_declare",
        amqp_queue_declare(
            conn.get(),
            m_opts.conn.channel,
            amqp_cstring_bytes(m_opts.queue.name.c_str()),
            m_opts.queue.passive,
            m_opts.queue.durable,
            m_opts.queue.name.empty() ? true : m_opts.queue.exclusive, // private client reply queue is always empty
            m_opts.queue.auto_delete,
            amqp_empty_table),
        conn.get(),
        TTransportException::INTERNAL_ERROR);

    // capture the queue name if it was empty which means broker makes a random name for it
    if (m_opts.queue.name.empty())
    {
        m_opts.queue.name.assign(static_cast<char *>(qdo->queue.bytes), qdo->queue.len);
    }

    // on the server side only, if a specific exchange was used then bind to it
    // the client side private reply queue always goes through the default exchange
    if (m_consumer && !m_opts.exch.name.empty())
    {
        (void) checkNotNull(
           "amqp_queue_bind",
            amqp_queue_bind(
                conn.get(),
                m_opts.conn.channel,
                amqp_cstring_bytes(m_opts.queue.name.c_str()),
                amqp_cstring_bytes(m_opts.exch.name.c_str()),
                amqp_cstring_bytes(m_opts.queue.name.c_str()),
                amqp_empty_table),
            conn.get(),
            TTransportException::INTERNAL_ERROR);
    }

    // prepare to consume on our inbound queue
    checkNotNull(
       "amqp_basic_consume",
        amqp_basic_consume(
            conn.get(),
            m_opts.conn.channel,
            amqp_cstring_bytes(m_opts.queue.name.c_str()),
            amqp_empty_bytes, // consumer_tag - not sure what to set this to
            false, // no_local
            true, // no_ack
            m_consumer ? m_opts.queue.exclusive : true, // exclusive
            amqp_empty_table),
        conn.get(),
        TTransportException::INTERNAL_ERROR);

    // everything was successful, so we'll keep the connection
    m_conn = conn;
}

void TRabbitMQSession::close()
{
    // idempotent
    if (m_conn)
    {
        nack_if();

        (void)amqp_channel_close(m_conn.get(), m_opts.conn.channel, AMQP_REPLY_SUCCESS);
        (void)amqp_connection_close(m_conn.get(), AMQP_REPLY_SUCCESS);
        m_conn.reset();

        m_reply_to.clear();
        m_delivery_tag = 0;
        m_rbuf.clear();
        m_rpos = 0;
        m_wbuf.clear();
    }
}

uint32_t TRabbitMQSession::read_virt(uint8_t* buf, uint32_t len)
{
    if (!m_conn)
    {
        throw TTransportException(TTransportException::NOT_OPEN, "not connected");
    }

    // message reads may come in as (4 bytes), (remaining bytes)
    // the first will buffer the entire message and return 4 bytes
    // subsequent reads need to send back as much as they can until
    // the whole message has been consumed.
    if (!m_rbuf.empty())
    {
        return give_buf(buf, len);
    }

    // @@@ prototype implementation restrictions: we don't have a way to pass
    //     the reply-to back up and have it passed back down as part of a reply
    //     so for now this class only supports one outstanding request, sorry
    if (m_consumer && !m_reply_to.empty())
    {
        throw TTransportException(TTransportException::INTERNAL_ERROR,
            "attempt to read while processing a message - did you use a threaded server or oneway call?");
    }

    // We consume messages, discarding irrelevant things like acks, until we get something useful
    for (;;)
    {
        amqp_envelope_t env;
        memset(&env, 0, sizeof(amqp_envelope_t));

        if (!checkRpcReply(
           "amqp_consume_message",
            amqp_consume_message(
                m_conn.get(),
                &env,
                NULL, // TODO: add support for recv timeout
                0),
            TTransportException::INTERNAL_ERROR))
        {
            amqp_destroy_envelope(&env);
            continue; // false means we discarded something irrelevant so call it again
        } 

        // verify the message came from a thrift endpoint
        std::string contentType(static_cast<char *>(env.message.properties.content_type.bytes),
                                                    env.message.properties.content_type.len);
        if (!(env.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) || "application/thrift" != contentType)
        {
            throw TTransportException(TTransportException::CORRUPTED_DATA, "invalid content type: " + contentType);
        }
        
        // If this is a request that requires a response, the reply-to property will be set.
        // Otherwise we are going to acknowledge it right away since it is a oneway request.
        if (env.message.properties._flags & AMQP_BASIC_REPLY_TO_FLAG)
        {
            m_reply_to.assign(static_cast<char *>(env.message.properties.reply_to.bytes),
                                                  env.message.properties.reply_to.len);
        }

        // save the delivery tag so we can acknowledge it appropriately
        m_delivery_tag = env.delivery_tag;

        m_rbuf.resize(env.message.body.len);
        memcpy(&m_rbuf[0], env.message.body.bytes, env.message.body.len);

        amqp_destroy_envelope(&env);
        break;
    } 

    amqp_maybe_release_buffers(m_conn.get());

    return give_buf(buf, len);
}

uint32_t TRabbitMQSession::give_buf(uint8_t *buf, uint32_t buflen)
{
    char *ptr = &m_rbuf[m_rpos];
    uint32_t left = m_rbuf.size() - m_rpos;
    uint32_t len = std::min(left, buflen);
    ::memcpy(buf, ptr, len);
    m_rpos += len;
    left -= len;
    if (!left)
    {
        m_rbuf.clear();
        m_rpos = 0;
    }
    return len;
}

uint32_t TRabbitMQSession::readEnd()
{
    if (m_reply_to.empty())
    {
        // this is not a request that will generate a response, so acknowledge it on read
        ack_if();
    }

    // @@@ TODO: here is where we will move the reply-to into a hint and return
    //           it to the caller; eventually it can be passed back down to us
    //           in the response (writeEnd) so we know where to send the reply

    return 0;
}

void TRabbitMQSession::ack_if()
{
    if (m_delivery_tag)
    {
        checkResult(
           "amqp_basic_ack",
            amqp_basic_ack(
                m_conn.get(),
                m_opts.conn.channel,
                m_delivery_tag,
                false),
            TTransportException::INTERNAL_ERROR);
        
        m_delivery_tag = 0;
    }
}

void TRabbitMQSession::nack_if()
{
    if (m_delivery_tag)
    {
        checkResult(
           "amqp_basic_nack",
            amqp_basic_nack(
                m_conn.get(),
                m_opts.conn.channel,
                m_delivery_tag,
                false,
                true),
            TTransportException::INTERNAL_ERROR);

        m_delivery_tag = 0;
    }
}

void TRabbitMQSession::write_virt(const uint8_t* buf, uint32_t len)
{
    std::vector<char>::size_type olen = m_wbuf.size();
    m_wbuf.resize(olen + len);
    memcpy(&m_wbuf[olen], buf, len);
}

uint32_t TRabbitMQSession::writeEnd()
{
    if (m_wbuf.size() > std::numeric_limits<uint32_t>::max())
    {
        throw TTransportException(TTransportException::INTERNAL_ERROR, "data size exceeded internal limits");
    }

    return static_cast<uint32_t>(m_wbuf.size());
}

void TRabbitMQSession::flush()
{
    if (!m_conn)
    {
        throw TTransportException(TTransportException::NOT_OPEN, "not connected");
    }

    amqp_basic_properties_t props;
    memset(&props, 0, sizeof(amqp_basic_properties_t));
    std::string contentType("application/thrift");
    props._flags |= AMQP_BASIC_CONTENT_TYPE_FLAG;
    props.content_type = amqp_cstring_bytes(contentType.c_str());
    if (!m_consumer)
    {
        props._flags |= AMQP_BASIC_REPLY_TO_FLAG;
        props.reply_to = amqp_cstring_bytes(m_opts.queue.name.c_str());
    }

    amqp_bytes_t body;
    body.bytes = &m_wbuf[0];
    body.len = m_wbuf.size();

    checkResult(
       "amqp_basic_publish",
        amqp_basic_publish(
            m_conn.get(),
            m_opts.conn.channel,
            // consumer always uses default exchange to publish rpc reply to producer:
            m_opts.exch.name.empty() ? amqp_empty_bytes : amqp_cstring_bytes(m_opts.exch.name.c_str()),
            // if this is a reply, send to the producer-specified private reply queue
            // otherwise this is a publisher to send it to the consumer's request queue:
            !(m_reply_to.empty()) ? amqp_cstring_bytes(m_reply_to.c_str()) : amqp_cstring_bytes(m_opts.rpc.sqn.c_str()),
            // mandatory: true means if it can't be delivered into a queue, oh well:
            false, 
            // immediate: false means if it can't be delivered immediately, oh well:
            false, 
            &props,
            body),
        TTransportException::INTERNAL_ERROR);

    amqp_maybe_release_buffers(m_conn.get());

    m_wbuf.resize(0);
}

const std::string TRabbitMQSession::getOrigin()
{
    std::stringstream ss;
    ss << "amqp";
    if (m_opts.tls.enabled)
        ss << "s";
    ss << "://" << m_opts.conn.host << ":" << m_opts.conn.port << "/";
    if (m_opts.conn.vhost.empty() || m_opts.conn.vhost.at(0) != '/')
        ss << "/";
    ss << m_opts.conn.vhost;
    return ss.str();
}

const TRabbitMQSessionOptions& TRabbitMQSession::getOptions() const
{
    return m_opts;
}

// --- error handling ---

static std::string makeErrPreamble(const std::string& callname, int ret)
{
    std::stringstream ss;
    ss << callname << ": [" << ret << "] ";
    return ss.str();
}

void TRabbitMQSession::checkResult(const std::string& callname, int ret, TTransportException::TTransportExceptionType type)
{
    if (0 == ret)
    {
        return;
    }
    else if (ret < 0)
    {
        // TODO: if the error is AMQP_STATUS_TIMER_FAILURE do we want it to be a TIMED_OUT TTransportExceptionType?
        throw TTransportException(type, makeErrPreamble(callname, ret) + amqp_error_string2(ret));
    }
    else
    {
        char buf[1024];
        throw TTransportException(type, makeErrPreamble(callname, ret) + ::strerror_r(ret, buf, 1024));
    }
}

template<class _T>
std::string getReplyText(const _T& info)
{
    const char *method_name = amqp_method_name(((info.class_id << 16) | info.method_id));
      
    if (info.reply_text.bytes)
        return method_name + std::string(" caused ") + std::string(static_cast<char *>(info.reply_text.bytes), info.reply_text.len);
    else
        return method_name + std::string(" caused ") + std::string(amqp_error_string2(info.reply_code));
}

// Reference: https://github.com/alanxz/rabbitmq-c/blob/master/librabbitmq/amqp.h (amqp_login)
bool TRabbitMQSession::checkRpcReply(const std::string& callname, const amqp_rpc_reply_t& reply, TTransportException::TTransportExceptionType type)
{
    amqp_frame_t frame;
    frame.frame_type = 0;
    frame.payload.method.id = 0;

    switch (reply.reply_type)
    {
        case AMQP_RESPONSE_NORMAL:
            return true;

        case AMQP_RESPONSE_LIBRARY_EXCEPTION:
            if (AMQP_STATUS_UNEXPECTED_STATE == reply.library_error)
            {
                // this basically handles errors from amqp_consume_message
                checkResult(callname, amqp_simple_wait_frame(m_conn.get(), &frame), type);

                if (AMQP_FRAME_METHOD == frame.frame_type)
                {
                    switch (frame.payload.method.id)
                    {
                        case AMQP_BASIC_ACK_METHOD:
                            // We do not care about ACKs
                            return false; // discard and tell caller to retry
                        
                        case AMQP_BASIC_RETURN_METHOD:
                            // A previously sent message could not be delivered
                            // We must read it and dispose of it if we don't care
                            {
                                amqp_message_t message;
                                memset(&message, 0, sizeof(amqp_message_t));
                                checkRpcReply(
                                   "amqp_read_message",
                                    amqp_read_message(m_conn.get(), frame.channel, &message, 0),
                                    TTransportException::INTERNAL_ERROR);
                                amqp_destroy_message(&message);
                                return false; // discard and tell caller to retry
                            }

                        case AMQP_CHANNEL_CLOSE_METHOD:
                            throw TTransportException(TTransportException::INTERNAL_ERROR, "channel closed by broker");

                        case AMQP_CONNECTION_CLOSE_METHOD:
                            throw TTransportException(TTransportException::END_OF_FILE, "connection closed by broker");
                            
                        default:
                            // unknown
                            break;
                    }
                }
            }
            break; 

        case AMQP_RESPONSE_SERVER_EXCEPTION:
            if (reply.reply.id == AMQP_CHANNEL_CLOSE_METHOD) {
                amqp_channel_close_t closeInfo = *(reinterpret_cast<amqp_channel_close_t *>(reply.reply.decoded));
                TTransportException ex(type, makeErrPreamble(callname, closeInfo.reply_code) + getReplyText(closeInfo));
                close();
                throw ex;
            } else if (reply.reply.id == AMQP_CONNECTION_CLOSE_METHOD) {
                amqp_connection_close_t closeInfo = *(reinterpret_cast<amqp_connection_close_t *>(reply.reply.decoded));
                TTransportException ex(type, makeErrPreamble(callname, closeInfo.reply_code) + getReplyText(closeInfo));
                close();
                throw ex;
            }
        default:
            break;
    }

    std::stringstream ss;
    ss << "unknown/unhandled rpc response case: ";
    ss << "reply_type=" << reply.reply_type << "; ";
    ss << "library_error=" << reply.library_error << "; ";
    ss << "frame_type=" << frame.frame_type << "; ";
    ss << "frame_payload_method_id=" << frame.payload.method.id;

    throw TTransportException(type, ss.str());
}

}
}
}

