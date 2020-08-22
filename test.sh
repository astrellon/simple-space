#!/bin/sh
mkdir -p build_test
cd build_test
cmake .. -DMAKE_TESTS=True
make -j12

if [ $? -eq 0 ]; then
    rm -rf ./data
    cp -rf ../tests/data ./
    ./space_tests
fi
