cmake_minimum_required(VERSION 3.5)

# Definitions
set(PROJECT_NAME_STR point_vision_pipeline_framework)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_BUILD_TYPE Debug)

# Project title and define versioning
project(${PROJECT_NAME_STR})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ${SRC_FILES} -ltbb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 --coverage ${SRC_FILES} -ltbb")

# Gather all sources except the main entry point

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE INC_FILES include/**/*.h include/**/*.hpp)
#file(GLOB_RECURSE INC_FILES include/*.h)
file(GLOB_RECURSE SRC_FILES src/**/*.cpp)
#file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Dependencies
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

 find_package(TBB REQUIRED)
 find_package(Boost COMPONENTS unit_test_framework system filesystem REQUIRED)
 find_package(OpenCV REQUIRED)
 find_package(rapidjson REQUIRED)

# Unit tests
add_subdirectory(unit_test)
add_subdirectory(system_test)

# Assign the include directories
include_directories (${Boost_INCLUDE_DIRS})


# Build
set(SOURCE_FILES src/main.cpp ${INC_FILES} ${SRC_FILES})
#set(SOURCE_FILES src/main.cpp ${SRC_FILES})
add_executable(point_vision_pipeline_framework ${SOURCE_FILES})
target_link_libraries(point_vision_pipeline_framework ${Boost_LIBRARIES})

