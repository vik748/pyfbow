# Install fbow
CWD=`pwd`
git clone --depth 1 https://github.com/rmsalinas/fbow fbow
mkdir -p fbow/build
cd fbow/build

cmake -DBUILD_SHARED_LIBS=OFF \
      -DCMAKE_INSTALL_PREFIX=$CWD/fbow_bin \
      -DCMAKE_CXX_FLAGS="-fPIC" \
      -DCMAKE_C_FLAGS="-fPIC" .. && make && make install 
cd ../..
