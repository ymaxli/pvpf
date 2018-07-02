#!/bin/bash
cmake build
cd build
find . -name "*.gcda" | xargs rm -rf
make
cd system_test
if [ $1 ]; then
    ./point_vision_pipeline_framework_system_tests $1
else
    ./point_vision_pipeline_framework_system_tests
fi