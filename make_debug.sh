#!/bin/sh
mkdir -p debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j12

cp -rf ../data ./
