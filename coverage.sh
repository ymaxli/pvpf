#!/bin/bash
cd build/unit_test/CMakeFiles/pvpf_unit_tests.dir
lcov --capture --directory . --output-file test.info --test-name test
lcov --remove test.info '/usr/local/include/*' 'v1' '*/pvpf/unit_test' '*/include/core' -o test_filtered.info
genhtml test_filtered.info --output-directory output --title "a simple test" --show-details --legend
cp -r output ../../../coverage