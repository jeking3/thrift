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

#define BOOST_TEST_MODULE TRabbitMQSessionTest
#include <boost/test/auto_unit_test.hpp>

#include <thrift/transport/TRabbitMQSession.h>

using apache::thrift::transport::TRabbitMQSession;
using apache::thrift::transport::TRabbitMQSessionOptions;
using apache::thrift::transport::TTransportException;

// assumes RabbitMQ running on standard port, all defaults (5672, guest/guest)

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

BOOST_AUTO_TEST_CASE(test_open_close_server)
{
    TRabbitMQSession rmq(TRabbitMQSessionOptions().connHost("rabbitmq").queueName("test_open_close").queueExclusive(true));
    rmq.open();
    BOOST_CHECK_THROW(rmq.open(), TTransportException); // open() is not idempotent
    rmq.close();
    rmq.close(); // close() is idempotent
}

BOOST_AUTO_TEST_CASE(test_open_close_client)
{
    TRabbitMQSession rmq(TRabbitMQSessionOptions().connHost("rabbitmq").rpcSqn("test_open_close").queueExclusive(true));
    rmq.open();
    BOOST_TEST_MESSAGE("rabbitmq private reply queue name: " + rmq.getOptions().queue.name);
    BOOST_CHECK(!rmq.getOptions().queue.name.empty());
    rmq.close();
}

BOOST_AUTO_TEST_CASE(test_ssltls_nyi)
{
    TRabbitMQSession uut(TRabbitMQSessionOptions().queueName("test_ssltls_nyi").tlsEnabled(true));
    BOOST_CHECK_THROW(uut.open(), TTransportException);
}

BOOST_AUTO_TEST_CASE(test_roundtrip)
{
    TRabbitMQSession producer(TRabbitMQSessionOptions().connHost("rabbitmq").rpcSqn("test_roundtrip").queueExclusive(true));
    producer.open();

    TRabbitMQSession consumer(TRabbitMQSessionOptions().connHost("rabbitmq").queueName("test_roundtrip").queueExclusive(true));
    consumer.open();

    BOOST_TEST_MESSAGE("writing request from producer to consumer");
    std::string data("abcdefgABCDEFG123321");
    producer.write_virt((const uint8_t *)(data.c_str()), data.size());
    BOOST_CHECK_EQUAL(producer.writeEnd(), 20);
    producer.flush();
    BOOST_TEST_MESSAGE("wrote   request from producer to consumer");

    BOOST_TEST_MESSAGE("reading request from producer to consumer");
    std::vector<uint8_t> rbuf;
    rbuf.resize(data.size());
    uint32_t readlen = consumer.read_virt(&rbuf[0], data.size());
    BOOST_TEST_MESSAGE("read    request from producer to consumer");

    BOOST_CHECK_EQUAL(readlen, data.size());
    BOOST_CHECK_EQUAL(std::string(reinterpret_cast<char *>(&rbuf[0]), rbuf.size()), data);
    consumer.readEnd();

    BOOST_TEST_MESSAGE("writing reply   from consumer to producer");
    std::string reply("987789XVYXVYxyzxyz!");
    consumer.write_virt((const uint8_t *)(reply.c_str()), reply.size());
    BOOST_CHECK_EQUAL(consumer.writeEnd(), 19);
    consumer.flush();
    BOOST_TEST_MESSAGE("wrote   reply   from consumer to producer");

    BOOST_TEST_MESSAGE("reading reply   from consumer to producer");
    rbuf.clear();
    rbuf.resize(reply.size());
    readlen = producer.read_virt(&rbuf[0], reply.size());
    BOOST_TEST_MESSAGE("read    reply   from consumer to producer");

    BOOST_CHECK_EQUAL(readlen, reply.size());
    BOOST_CHECK_EQUAL(std::string(reinterpret_cast<char *>(&rbuf[0]), rbuf.size()), reply);
    producer.readEnd();

    consumer.close();
    producer.close();
}

BOOST_AUTO_TEST_SUITE_END()
