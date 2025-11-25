#! /bin/sh
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_SAN=ON
cmake --build build
ctest --test-dir build
