#!/bin/bash

SYSTEM=$(uname -s)
CPU_CORES=1

if [ "${SYSTEM}" == "Darwin" ]; then
    CPU_CORES=$(sysctl hw.ncpu | cut -d ':' -f 2)
elif [ "${SYSTEM}" == "Linux" ]; then
    CPU_CORES=$(grep -c ^processor /proc/cpuinfo)
fi

# Get the source code of CLAPACK
wget http://www.netlib.org/clapack/clapack.tgz

# Decompress the source
tar xvf clapack.tgz

# Build CLAPACK
cd CLAPACK-3.2.1/
cp make.inc.example make.inc
make -j ${CPU_CORES} lib
make -j ${CPU_CORES} lapacklib
cd -

# Copy libraries to lib directory
mkdir -p lib
find CLAPACK-3.2.1/ -name blas_LINUX.a -exec cp {} ./lib/libblas.a \;
find CLAPACK-3.2.1/ -name lapack_LINUX.a -exec cp {} ./lib/liblapack.a \;
find CLAPACK-3.2.1/ -name libf2c.a -exec cp {} ./lib/ \;

