#!/bin/bash
cmake build
cd build
find . -name "*.gcda" | xargs rm -rf
cd unit_test
make
if [ $1 ]; then
    ./pvpf_unit_tests $1
else
    ./pvpf_unit_tests
fi