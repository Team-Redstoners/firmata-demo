#!/bin/bash
cmake CMakeLists.txt -B build
cmake --build build
cd build
./main
cd ..
