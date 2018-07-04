#!/bin/bash
cmake build
cd build
find . -name "*.gcda" | xargs rm -rf
cd system_test
make
if [ $1 ]; then
    ./pvpf_system_tests $1
else
    ./pvpf_system_tests
fi