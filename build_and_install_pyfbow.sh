#!/bin/bash

set -x

cd ${0%%$(basename $0)}
CWD=`pwd`

cd install

chmod +x build_and_install_fbow.sh && ./build_and_install_fbow.sh || exit 1

cd ..

mkdir build
cd build

cmake -DCMAKE_PREFIX_PATH=$CWD/install/fbow_bin -DBOOST_ROOT=$CONDA_PREFIX ../src
make -j$(nproc)

if [[ "$OSTYPE" == "darwin"* ]]; then
    ln -s pyfbow.dylib pyfbow.so
fi
