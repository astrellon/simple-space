#!/bin/sh
mkdir -p build
cd build
cmake ..
make -j12

makeresult=$?

if [ $makeresult -eq 0 ]; then
    cp -rf ../data ./
else
    return $makeresult
fi
