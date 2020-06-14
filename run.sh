#!/bin/sh
./make.sh
if [ $? -eq 0 ]; then
    ./build/space
fi
