Point Vision Pipeline Framework
================


The project structure is provided below:

```
.
├── bin/                                # Folder for built application binary
│   └── point_vision_pipeline_framework # Built application executable
├── cmake/                              # 
│   └── module/                         # Used to find dependencies.
├── build/                              # Build artifacts for build and tests.
│   ├── coverage                        # Artifact of coverage reports
│   ├── system_test                     # Build artifacts for system tests
│   └── unit_test                       # Build artifacts for unit tests
├── include/                            # Include header files.
│   ├── core                            # Include library from core.
│   └── pvpf                            # Header files from pvpf.
├── src/                                # Folder for all source files
│   ├── config/                         # source files related to configuration
│   └── main.cpp                        # Main entry of the application
├── system_test/                        # System test folder for all system tests
│   ├── test_images/                    # images for system tests.
│   └── CMakeLists.txt                  # Compile script for unit tests
├── unit_test/                          # Unit test folder for all system tests
│   ├── test_images/                    # images for system tests.
│   └── CMakeLists.txt                  # Compile script for unit tests
├── coverage.sh                         # Generate coverage report script
├── mcov.sh                             # Generate coverage report for human script
├── msystem_test.sh                     # System test script
├── munit_test.sh                       # Unit test script
└── CMakeLists.txt                      # Main compile script
```

## Dependencies

This project was built and tested on the Mac OS X platform with the
following dependencies:

* CMake >= 3.10
* Boost >= 1.66
* TBB >= 4.4
* CUDA >= 7.6.31
* Rapidjson >= 1.1.0

## Instructions

Do as you would any CMake project to compile:

```
mkdir build
cd build
cmake ..

make
```

Run application:

```
cd bin
./point_vision_pipeline_framework
```

Run unit test suite:

```
./munit_test.sh --run_test <test_suite>/<test_case>
```

Run system test suite:

```
./msystem_test.sh --run_test <test_suite>/<test_case>
```

Generate coverage report

```
./mcov.sh
```
## References

* [CMake: An introduction](https://www.cs.swarthmore.edu/~adanner/tips/cmake.php)
* [CMake/Testing with CTest](https://cmake.org/Wiki/CMake/Testing_With_CTest)
* [Get to know the Boost unit test framework](http://www.ibm.com/developerworks/aix/library/au-ctools1_boost/index.html)
