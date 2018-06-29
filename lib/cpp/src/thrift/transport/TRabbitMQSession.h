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

#ifndef _THRIFT_TRANSPORT_TRABBITMQ_H_
#define _THRIFT_TRANSPORT_TRABBITMQ_H_ 1

#include <amqp.h>
// TODO: make it boost::move able
#include <boost/noncopyable.hpp>
#include <stdint.h>
#include <string>
#include <thrift/transport/TRabbitMQSessionOptions.h>
#include <thrift/transport/TVirtualTransport.h>

namespace apache {
namespace thrift {
namespace transport {

/**
 * \brief RabbitMQ session transport
 *
 * A message bus consumer is equivalent to a thrift server.
 * A message bus publisher is equivalent to a thrift client.
 *
 * NOTES:
 *
 * When a connection is established, an exchange is created (or reused)
 * based on the session options.  In addition, a queue is created and bound
 * to the exchange using the given routing key in the session options.  If
 * this is a consumer, this is a shared request queue.  Multiple servers on the
 * same queue will pull requests in a round-robin fashion.  If this is a
 * producer, is a private reply queue.
 *
 * To use an asynchronous call pattern which supports one or more publishers
 * to one or more consumers, declare your thrift service with oneway methods.
 * To leverage multiple consumers in a scale-out mode, where only one consumer
 * will receive a request, ensure they all use the same queue name and that
 * the queue is not set to "exclusive".
 * To target multiple consumers simultaneously (for parallel processing),
 * ensure they all use the same exchange, and ensure the exchange is a
 * "fanout" type which means it will deliver to all queues.
 * 
 * To use a synchronous RPC pattern, declare your thrift service normally.
 * Configure your consumer and publisher to use the same exchange (or leave
 * unconfigured to use the default exchange).
 * Configure your consumer (server) to have a well-known queue name.
 * Configure your publisher (client) to have that name in the rpcSqn().
 */
class TRabbitMQSession
  : private boost::noncopyable
{
  public:
    /**
     * \brief Construct a RabbitMQ transport endpoint.
     * \param[in]  opts   the session options - these are copied
     */
    TRabbitMQSession(const TRabbitMQSessionOptions& opts);

    virtual ~TRabbitMQSession();

    virtual bool isOpen() /* override */;
    virtual bool peek() /* override */;
    virtual void open() /* override */;
    virtual void close() /* override */;

    virtual uint32_t read_virt(uint8_t* /* buf */, uint32_t /* len */) /* override */;
    virtual uint32_t readEnd() /* override */;

    virtual void write_virt(const uint8_t* /* buf */, uint32_t /* len */) /* override */;
    virtual uint32_t writeEnd() /* override */;

    virtual void flush() /* override */;

    virtual const std::string getOrigin() /* override */;

    const TRabbitMQSessionOptions& getOptions() const;

  private: // methods
    /**
     * \brief Check the result is not NULL
     * Some of the APIs return a structure pointer on success or NULL.
     *
     * \param[in]  callname  the call that was made
     * \param[in]  res       the result
     * \param[in]  type      exception type to use if an error occurred
     * \throws TTransportException(type) if the return code was not AMQP_STATUS_OK
     */ 
    template<class _STRUCT>
    _STRUCT * checkNotNull(const std::string& callname, _STRUCT *ptr, amqp_connection_state_t state,
        TTransportException::TTransportExceptionType type)
    {
        if (ptr)
            return ptr;

        // internally amqp_framing.c uses amqp_simple_rpc_decoded for methods
        // that can return NULL.  The documentation for amqp_simple_rpc_decoded
        // documents how to handle this situation.

        checkRpcReply(callname, amqp_get_rpc_reply(state), type);
        return NULL; /* notreached */
    }

    /**
     * \brief Check the result for an error and throw
     * Some of the APIs return an integer error code:
     *    - 0 is okay
     *    - anything less than 0 is an AMQP error,
     *    - anything greater than 0 is a POSIX error.
     * 
     * \param[in]  callname  the call that was made
     * \param[in]  res       the result
     * \param[in]  type      exception type to use if an error occurred
     * \throws TTransportException(type) if the return code was not AMQP_STATUS_OK
     */
    void checkResult(const std::string& callname, int res,
        TTransportException::TTransportExceptionType type);

    /**
     * \brief Handle an amqp_rpc_reply_t response (amqp_login, etc.)
     * The handling instructions for this are quite specific:
     * https://github.com/alanxz/rabbitmq-c/blob/master/librabbitmq/amqp.h#L1729
     * 
     * \param[in]  callname  the call that was made
     * \param[in]  reply     the rpc reply
     * \param[in]  type      exception type to use if an error occurred
     * \returns  true  if there was no error,
     *           false if a message was discarded, so make the call again
     * \throws TTransportExceptioni(type) if the response was bad
     */
    bool checkRpcReply(const std::string& callname, const amqp_rpc_reply_t& reply,
        TTransportException::TTransportExceptionType type);

    /**
     * \brief Send as much buffered data as possible.
     * \param[in]  buf       the buffer
     * \param[in]  buflen    the maximum length to write in the buffer
     * \returns              the amount written
     */
    uint32_t give_buf(uint8_t *buf, uint32_t buflen);

    /**
     * \brief Acknowledge the last message as received if we haven't replied
     */
    void ack_if();

    /** 
     * \brief Reject the last message received if we haven't replied
     */
    void nack_if();

  private: // members
    // Session options
    TRabbitMQSessionOptions m_opts;

    // Is this a consumer?
    bool m_consumer;

    // Connection
    stdcxx::shared_ptr<amqp_connection_state_t_> m_conn;
    
    //! Current message "reply-to" value
    //! \note this is purely for prototyping
    //!       it means the consume (server) can only process one message at a time
    //!       this is because transports have always been stream based, which means
    //!       there's always only one entity on the other end.  With message bus,
    //!       thrift needs to change so a hint can be passed up, carried around
    //!       with the request, and handed back down on the reply.  This is a major
    //!       change.
    std::string m_reply_to;

    // The delivery tag to acknowledge before replying
    uint64_t m_delivery_tag;

    // The current message buffer for reading
    std::vector<char> m_rbuf;

    // The current message buffer position for reading
    std::vector<char>::size_type m_rpos;

    // The current message buffer for writing
    std::vector<char> m_wbuf;
};

}
}
}

#endif
