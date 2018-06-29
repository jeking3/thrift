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

#include <string.h>
#include <thrift/stdcxx.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TRabbitMQSessionOptions.h>
#include <thrift/transport/TTransportException.h>

namespace apache {
namespace thrift {
namespace transport {

TRabbitMQSessionOptions::TRabbitMQSessionOptions()
{
    initialize();
}

TRabbitMQSessionOptions::TRabbitMQSessionOptions(const amqp_connection_info& info)
{
    initialize();

    connHost(info.host);
    connPort(info.port);
    connUser(info.user);
    connPass(info.password);
    connVHost(info.vhost);
    tlsEnabled(info.ssl);
}

TRabbitMQSessionOptions::TRabbitMQSessionOptions(const std::string& uri)
{
    initialize();

    amqp_connection_info info;

    // amqp_parse_url modifies the URL so we work with a copy
    stdcxx::shared_ptr<char> cUri(::strdup(uri.c_str()), free);
    int rc = amqp_parse_url(cUri.get(), &info);
    if (AMQP_STATUS_OK != rc)
    {
        throw TTransportException(TTransportException::BAD_ARGS);
    }

    connHost(info.host);
    connPort(info.port);
    connUser(info.user);
    connPass(info.password);
    connVHost(info.vhost);
    tlsEnabled(info.ssl);
}

void
TRabbitMQSessionOptions::initialize()
{
    connHost("localhost");
    connPort(AMQP_PROTOCOL_PORT);
    connUser("guest");
    connPass("guest");
    connVHost("/"); // AMQP_DEFAULT_VHOST
    connChannel(1);
    connFrameMax(TFramedTransport::DEFAULT_MAX_FRAME_SIZE);
    connHeartbeat(AMQP_DEFAULT_HEARTBEAT);

    tlsEnabled(false);
    tlsMinVer(AMQP_TLSv1_2);
    tlsMaxVer(AMQP_TLSvLATEST);
    tlsVerifyHost(true);

    exchType("direct");
    exchPassive(false);
    exchDurable(false);
    exchAutoDelete(true);

    queuePassive(false);
    queueDurable(false);
    queueExclusive(false);
    queueAutoDelete(true);
}

}
}
}

