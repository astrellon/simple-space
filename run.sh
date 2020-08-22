#!/bin/sh
./make.sh

if [ $? -eq 0 ]; then
    cd build
    rm -rf ./data
    cp -rf ../data ./
    ./space
fi