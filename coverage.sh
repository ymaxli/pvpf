#!/bin/bash
cd build/unit_test/CMakeFiles/pvpf_unit_tests.dir
lcov --capture --directory . --output-file test.info --test-name test
genhtml test.info --output-directory output --title "a simple test" --show-details --legend
cp -r output ../../../coverage