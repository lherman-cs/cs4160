#!/bin/bash

VERSION="2.2.6"
UPSTREAM="https://github.com/libexpat/libexpat/releases/download/R_`echo ${VERSION} | tr . _`/expat-${VERSION}.tar.bz2"

cd /tmp
wget "${UPSTREAM}"
tar -xf "expat-${VERSION}.tar.bz2"
cd "expat-${VERSION}"
mkdir build && cd build
emconfigure cmake .. && emmake make

# Prepare include
mkdir -p /vendor/include/expat
cp ../lib/*.h /vendor/include/expat/

# Prepare shared libraries
mkdir -p /vendor/lib/expat
cp lib* /vendor/lib/expat/