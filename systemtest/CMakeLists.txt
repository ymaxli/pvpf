cmake_minimum_required(VERSION 3.5)

# Project settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")
set(PROJECT_SYSTEM_TESTS_NAME ${PROJECT_NAME_STR}_system_tests)

# Dependencies
find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)

# Gather the header and source files
file(GLOB SYSTEM_TESTS_INC_FILES ${PROJECT_SOURCE_DIR}/systemtest/*.hpp)
file(GLOB SYSTEM_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/systemtest/*.cpp)

# Include paths
set(SYSTEM_TESTS_INCLUDES ${PROJECT_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIR})

# Assign the include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SYSTEM_TESTS_INCLUDES})

# Build unit tests
add_definitions(-DBOOST_TEST_DYN_LINK)
#add_executable(${PROJECT_SYSTEM_TESTS_NAME} ${SYSTEM_TESTS_SRC_FILES} $<TARGET_OBJECTS:demo_objects> )
add_executable(${PROJECT_SYSTEM_TESTS_NAME} ${SYSTEM_TESTS_SRC_FILES} ${SRC_FILES})
target_link_libraries(${PROJECT_SYSTEM_TESTS_NAME} ${Boost_LIBRARIES})

enable_testing()
add_test(DemoTest ${PROJECT_SYSTEM_TESTS_NAME})
