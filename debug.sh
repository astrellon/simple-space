#!/bin/sh
mkdir -p build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j12

cp -rf ../data ./
