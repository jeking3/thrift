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

#ifndef _THRIFT_TRANSPORT_TRABBITMQ_SESSION_OPTIONS_H_
#define _THRIFT_TRANSPORT_TRABBITMQ_SESSION_OPTIONS_H_ 1

extern "C" {
#include <amqp_ssl_socket.h>
}

#include <string>

namespace apache {
namespace thrift {
namespace transport {

/**
 * RabbitMQ session options contains all of the information
 * needed to negotiate a session with the message bus.
 *
 * On the server side (consumer), be sure to set queueName()
 * to a well-known value.  Choose a unique name that both the
 * server and client know.  This allows them to communicate.
 *
 * On the client side (producer) leave the queue name blank
 * and instead put the server's queue name into rpcSqn().
 * This allows the broker to forward client messages to the
 * server queue.  The client will create a private reply queue
 * and use the AMQP "reply.to" property so the server knows
 * how to route the reply back to the client.
 *
 * There can be multiple clients publishing requests to the
 * same server.  There can also be many servers listening
 * to the same queue in which case they will pick up messages
 * in a round-robin fashion.  There can also be many servers
 * listening to a fanout exchange which allows all the servers
 * to receive every message published to the exchange.
 */
class TRabbitMQSessionOptions
{
  public:
    /**
     * Initialize the RabbitMQ session options with defaults.
     */
    TRabbitMQSessionOptions();

    /**
     * Initialize the RabbitMQ session options.
     * \param[in]  info   connection information
     */
    TRabbitMQSessionOptions(const amqp_connection_info& info);

    /**
     * Initialize the RabbitMQ session options.
     * \param[in]  uri    an amqp uri of the form:
     *                    amqp[s]://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
     * \throws     TTransportException(BAD_ARGS)  if the uri is invalid
     */
    TRabbitMQSessionOptions(const std::string& uri);

    // connection data (note: AMQP_SASL_METHOD_PLAIN is always used)
    struct conn_
    {
	std::string host;             // host to connect to                    (default: localhost)
	int port;                     // port number                           (default: 5672)
	std::string user;             // username for amqp_login               (default: guest)
	std::string pass;             // password for amqp_login               (default: guest)
	std::string vhost;            // virtual host (amqp concept)           (default: /)
        int channel;                  // channel                               (default: 1)
        int frame_max;                // must match TFramedTransport!          (default: 256MB)
        int heartbeat;                // seconds between heartbeats            (default: 0)
    } conn;

    TRabbitMQSessionOptions& connHost           (const std::string& s) { conn.host            = s; return *this; }
    TRabbitMQSessionOptions& connPort           (int                n) { conn.port            = n; return *this; }
    TRabbitMQSessionOptions& connUser           (const std::string& s) { conn.user            = s; return *this; }
    TRabbitMQSessionOptions& connPass           (const std::string& s) { conn.pass            = s; return *this; }
    TRabbitMQSessionOptions& connVHost          (const std::string& s) { conn.vhost           = s; return *this; }
    TRabbitMQSessionOptions& connChannel        (int                n) { conn.channel         = n; return *this; }
    TRabbitMQSessionOptions& connFrameMax       (int                n) { conn.frame_max       = n; return *this; }
    TRabbitMQSessionOptions& connHeartbeat      (int                n) { conn.heartbeat       = n; return *this; }

    // ssl/tls data
    struct tls_
    {
        bool enabled;                 // secure?                               (default: false)
        amqp_tls_version_t min_ver;   // minimum TLS to allow                  (default: AMQP_TLSv1_2)
        amqp_tls_version_t max_ver;   // maximum TLS to allow                  (default: AMQP_TLSv1_LATEST)
        std::string ca_cert;          // CA certificate file
        std::string my_cert;          // certificate for this instance
        std::string my_key;           // certificate key
        bool verify_host;             // verify the remote hostname            (default: true)
    } tls;

    TRabbitMQSessionOptions& tlsEnabled         (bool               b) { tls.enabled          = b; return *this; }
    TRabbitMQSessionOptions& tlsMinVer          (amqp_tls_version_t v) { tls.min_ver          = v; return *this; }
    TRabbitMQSessionOptions& tlsMaxVer          (amqp_tls_version_t v) { tls.max_ver          = v; return *this; }
    TRabbitMQSessionOptions& tlsCACert          (const std::string& s) { tls.ca_cert          = s; return *this; }
    TRabbitMQSessionOptions& tlsMyCert          (const std::string& s) { tls.my_cert          = s; return *this; }
    TRabbitMQSessionOptions& tlsMyKey           (const std::string& s) { tls.my_key           = s; return *this; }
    TRabbitMQSessionOptions& tlsVerifyHost      (bool               b) { tls.verify_host      = b; return *this; }

    // exchange data (optional - leave alone to use the default exchange)
    struct exch_
    {
	std::string name;             // exchange name                         (default: "", or the default exchange)
	std::string type;             // exchange type, can be "direct",       (default: "direct")
                                      //   "fanout", "topic" (, or "match"?)
	bool passive;                 // not sure what this is for             (default: false)
	bool durable;                 // whether exchange survives restart     (default: false)
	bool auto_delete;             // delete when last usage closes         (default: true)
    } exch;

    TRabbitMQSessionOptions& exchName           (const std::string& s) { exch.name            = s; return *this; }
    TRabbitMQSessionOptions& exchType           (const std::string& s) { exch.type            = s; return *this; }
    TRabbitMQSessionOptions& exchPassive        (bool               b) { exch.passive         = b; return *this; }
    TRabbitMQSessionOptions& exchDurable        (bool               b) { exch.durable         = b; return *this; }
    TRabbitMQSessionOptions& exchAutoDelete     (bool               b) { exch.auto_delete     = b; return *this; }

    // queue data
    struct queue_
    {
	std::string name;             // queue name                            (default: "")
                                      //   on the server side, set this to be
                                      //     a well-known name, and then on the
                                      //     client put this name into the
                                      //     server's queue name (sqn) below.
                                      //   on the client side leave this blank
                                      //     so the client makes an anonymous
                                      //     reply queue and puts name here.
	bool passive;                 // not sure what this is for             (default: false)
	bool durable;                 // whether exchange survives restart     (default: false)
	bool exclusive;               // used by one connection, auto-deletes  (default: false)
                                      //   when that connection closes
                                      //   this is always true on a client's
                                      //   private reply queue
	bool auto_delete;             // delete on last consumer unsubscribe   (default: true)
    } queue;

    TRabbitMQSessionOptions& queueName          (const std::string& s) { queue.name           = s; return *this; }
    TRabbitMQSessionOptions& queuePassive       (bool               b) { queue.passive        = b; return *this; }
    TRabbitMQSessionOptions& queueDurable       (bool               b) { queue.durable        = b; return *this; }
    TRabbitMQSessionOptions& queueExclusive     (bool               b) { queue.exclusive      = b; return *this; }
    TRabbitMQSessionOptions& queueAutoDelete    (bool               b) { queue.auto_delete    = b; return *this; }

    // rpc data
    struct rpc_
    {
	std::string sqn;              // the server's queue name               (default: "")
                                      //   only set this on the clientt
    } rpc;

    TRabbitMQSessionOptions& rpcSqn             (const std::string& s) { rpc.sqn              = s; return *this; }

  private:
    /**
     * Initialize the defaults.
     */
    void initialize();
};

}
}
}

#endif

