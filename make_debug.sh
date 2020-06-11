#!/bin/sh
mkdir -p debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

cp -rf ../data ./
