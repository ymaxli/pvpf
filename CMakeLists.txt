cmake_minimum_required(VERSION 3.5)

# Definitions
set(PROJECT_NAME_STR dev_env)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Project title and define versioning
project(${PROJECT_NAME_STR})

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ltbb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Gather all sources except the main entry point
file(GLOB_RECURSE INC_FILES src/*/*.hpp)
file(GLOB_RECURSE SRC_FILES src/*/*.cpp)

# Dependencies
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# find_package(TBB REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
# find_package(OpenCV REQUIRED)
# find_package(rapidjson REQUIRED)

# Unit tests
add_subdirectory(unittest)
add_subdirectory(systemtest)

# Assign the include directories
include_directories (${Boost_INCLUDE_DIRS})


# Build
set(SOURCE_FILES src/main.cpp ${INC_FILES} ${SRC_FILES})
add_executable(dev_env ${SOURCE_FILES})
target_link_libraries(dev_env ${Boost_LIBRARIES})
