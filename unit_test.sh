#!/bin/bash
cmake build
cd build
find . -name "*.gcda" | xargs rm -rf
cd unit_test
make
if [ $1 ]; then
    ./point_vision_pipeline_framework_unit_tests $1
else
    ./point_vision_pipeline_framework_unit_tests
fi