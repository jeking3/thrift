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

#define BOOST_TEST_MODULE TRabbitMQSessionOptionsTest
#include <boost/test/auto_unit_test.hpp>

#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TRabbitMQSessionOptions.h>

using apache::thrift::transport::TFramedTransport;
using apache::thrift::transport::TRabbitMQSessionOptions;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

BOOST_AUTO_TEST_CASE(test_default_max_frame_size)
{
    // Ensure our default max frame size matches the frame transport default max size
    // Ultimately it's up to the consumer to make sure these are aligned if other values are used.
    TFramedTransport tft;
    TRabbitMQSessionOptions opts;
    BOOST_CHECK_EQUAL(opts.conn.frame_max, tft.getMaxFrameSize());
}

BOOST_AUTO_TEST_CASE(test_defaults)
{
    TRabbitMQSessionOptions opts;

    BOOST_CHECK_EQUAL(opts.conn.host, "localhost");
    BOOST_CHECK_EQUAL(opts.conn.port, 5672);
    BOOST_CHECK_EQUAL(opts.conn.user, "guest");
    BOOST_CHECK_EQUAL(opts.conn.pass, "guest");
    BOOST_CHECK_EQUAL(opts.conn.vhost, "/");
    BOOST_CHECK_EQUAL(opts.conn.channel, 1);
    BOOST_CHECK_EQUAL(opts.conn.frame_max, 256*1024*1024);
    BOOST_CHECK_EQUAL(opts.conn.heartbeat, 0);

    BOOST_CHECK_EQUAL(opts.tls.enabled, false);
    BOOST_CHECK_EQUAL(opts.tls.min_ver, AMQP_TLSv1_2);
    BOOST_CHECK_EQUAL(opts.tls.max_ver, AMQP_TLSvLATEST);
    BOOST_CHECK      (opts.tls.ca_cert.empty());
    BOOST_CHECK      (opts.tls.my_cert.empty());
    BOOST_CHECK      (opts.tls.my_key.empty());
    BOOST_CHECK_EQUAL(opts.tls.verify_host, true);

    BOOST_CHECK      (opts.exch.name.empty());
    BOOST_CHECK_EQUAL(opts.exch.type, "direct");
    BOOST_CHECK_EQUAL(opts.exch.passive, false);
    BOOST_CHECK_EQUAL(opts.exch.durable, false);
    BOOST_CHECK_EQUAL(opts.exch.auto_delete, true);

    BOOST_CHECK      (opts.queue.name.empty());
    BOOST_CHECK_EQUAL(opts.queue.passive, false);
    BOOST_CHECK_EQUAL(opts.queue.durable, false);
    BOOST_CHECK_EQUAL(opts.queue.exclusive, false);
    BOOST_CHECK_EQUAL(opts.queue.auto_delete, true);

    BOOST_CHECK      (opts.rpc.sqn.empty());
}

BOOST_AUTO_TEST_CASE(test_ctor_info)
{
    TRabbitMQSessionOptions def;
    def.connHost("host");
    def.connPort(1234);
    def.connUser("user");
    def.connPass("password");
    def.connVHost("vhost");
    def.tlsEnabled(true);

    amqp_connection_info info;
    info.host = const_cast<char *>(def.conn.host.c_str());
    info.port = 1234;
    info.user = const_cast<char *>(def.conn.user.c_str());
    info.password = const_cast<char *>(def.conn.pass.c_str());
    info.vhost = const_cast<char *>(def.conn.vhost.c_str());
    info.ssl = true;

    TRabbitMQSessionOptions opts(info);

    BOOST_CHECK_EQUAL(opts.conn.host, "host");
    BOOST_CHECK_EQUAL(opts.conn.port, 1234);
    BOOST_CHECK_EQUAL(opts.conn.user, "user");
    BOOST_CHECK_EQUAL(opts.conn.pass, "password");
    BOOST_CHECK_EQUAL(opts.conn.vhost, "vhost");
    BOOST_CHECK_EQUAL(opts.tls.enabled, true);
}

BOOST_AUTO_TEST_CASE(test_ctor_uri)
{
    const std::string uri("amqps://user:pass@host:1234/vhost");

    TRabbitMQSessionOptions opts(uri);

    BOOST_CHECK_EQUAL(opts.conn.host, "host");
    BOOST_CHECK_EQUAL(opts.conn.port, 1234);
    BOOST_CHECK_EQUAL(opts.conn.user, "user");
    BOOST_CHECK_EQUAL(opts.conn.pass, "pass");
    BOOST_CHECK_EQUAL(opts.conn.vhost, "vhost");
    BOOST_CHECK_EQUAL(opts.tls.enabled, true);
}
    
BOOST_AUTO_TEST_CASE(test_setters)
{
    // TODO: test all the setters
}

BOOST_AUTO_TEST_SUITE_END()

