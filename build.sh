#!/bin/bash

set -x

cd ${0%%$(basename $0)}
CWD=`pwd`

cd install

chmod +x fbow.sh && ./fbow.sh || exit 1

cd ..

mkdir build
cd build

cmake -DBUILD_STATICALLY_LINKED=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_PREFIX_PATH=$CWD/install/fbow_bin ../src
make
