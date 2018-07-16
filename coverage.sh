#!/bin/bash
rm -r build/coverage
cd build/unit_test/CMakeFiles/pvpf_unit_tests.dir
rm -r output

lcov --capture --directory . --output-file test.info --test-name test
lcov --remove test.info '/usr/local/include/*' '/Library/Developer/CommandLineTools/usr/include/c++/*' '*/unit_test/*' '*/include/core/*' '/usr/include/*' -o test_filtered.info

genhtml test_filtered.info --output-directory output --title "unit tests code coverage" --show-details --legend

cp -r output ../../../coverage