#!/bin/bash
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

set -e

function update_docker
{
  set -x
  local dockerver
  dockerver=$(docker --version | cut -d' ' -f3 | cut -d'.' -f1) 
  if [[ $dockerver -ge 18 ]]; then
    set +x
    return 0
  fi

  if [[ -z "$TRAVIS" ]]; then
    echo "not updating docker outside of travis - ensure yours is new enough"
    return 0
  fi

  # update docker - we're less than version 18
  echo "updating docker - we need a newer version for docker-compose"
  echo "old version:"
  docker --version || true
  sudo apt-get remove docker docker-engine docker.io docker-compose || true
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
  sudo add-apt-repository --yes \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
  sudo apt-get install docker-ce python3-pip
  sudo pip3 install docker-compose
  hash -r
  docker --version
  docker-compose version
  set +x
}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKER_TAG=$DOCKER_REPO:$DISTRO

printenv | sort

update_docker

docker run -e BUILD_LIBS="$BUILD_LIBS" $BUILD_ENV -v $(pwd):/thrift/src \
	-it $DOCKER_TAG build/docker/scripts/$SCRIPT $BUILD_ARG

