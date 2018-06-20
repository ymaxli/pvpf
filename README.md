PVPF
================


The project structure is provided below:

```
.
├── bin/                                # Folder for built application binary
│   └── cmake_boost_demo                # Built application executable
├── cmake/                              # CMake build folder
│   ├── shared/                         # CMake artifacts
│   └── test/                           # Compiled test folder
│       └── cmake_boost_demo_unit_tests # Built unit tests executable
├── src/                                # Folder for all source files
│   ├── shared/                         # Shared classes
│   └── main.cpp                        # Main entry of the application
├── test/                               # Test folder for all unit tests
│   ├── shared/                         # Shared classes
│   └── CMakeLists.txt                  # Compile script for unit tests
└── CMakeLists.txt                      # Main compile script
```

## Dependencies

This project was built and tested on the Mac OS X platform with the
following dependencies:

* CMake >= 3.5
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
./cmake_boost_demo
```

Run unit test suite:

```
cd build/test
ctest
```

## References

* [CMake: An introduction](https://www.cs.swarthmore.edu/~adanner/tips/cmake.php)
* [CMake/Testing with CTest](https://cmake.org/Wiki/CMake/Testing_With_CTest)
* [Get to know the Boost unit test framework](http://www.ibm.com/developerworks/aix/library/au-ctools1_boost/index.html)
