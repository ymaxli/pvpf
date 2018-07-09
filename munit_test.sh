#!/bin/bash
cmake build
cd build
find . -name "*.gcda" | xargs rm -rf
cd unit_test
make
./pvpf_unit_tests $*