# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Apache Thrift Docker build environment for testing
# builds of the C++ library on systems without any
# of the following optional third party libraries:
#
#   libevent
#   openssl
#   qt (any variety)
#   zlib
#

FROM buildpack-deps:yakkety-scm
MAINTAINER Apache Thrift <dev@thrift.apache.org>

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
`# General dependencies` \
      bison \
      build-essential \
      clang \
      cmake \
      debhelper \
      flex \
      pkg-config \
`# C++ dependencies` \
      libboost-dev \
      libboost-filesystem-dev \
      libboost-program-options-dev \
      libboost-system-dev \
      libboost-test-dev \
      libboost-thread-dev

RUN \
`# Clean up` \
    rm -rf /var/cache/apt/* && \
    rm -rf /var/lib/apt/lists/* && \
    rm -rf /tmp/* && \
    rm -rf /var/tmp/*

ENV THRIFT_ROOT /thrift
RUN mkdir -p $THRIFT_ROOT/src
COPY Dockerfile $THRIFT_ROOT/
WORKDIR $THRIFT_ROOT/src
