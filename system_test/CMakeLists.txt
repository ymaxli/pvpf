cmake_minimum_required(VERSION 3.5)

# Project settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")
set(PROJECT_SYSTEM_TESTS_NAME pvpf_system_tests)

# Dependencies
#find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)
#find_package(OpenCV REQUIRED)

# Gather the header and source files
file(GLOB SYSTEM_TESTS_INC_FILES ${PROJECT_SOURCE_DIR}/system_test/*.hpp)
file(GLOB SYSTEM_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/system_test/*.cpp)

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
target_link_libraries(${PROJECT_SYSTEM_TESTS_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${CMAKE_DL_LIBS} -ltbb)

enable_testing()
add_test(SYSTEMTEST ${PROJECT_SYSTEM_TESTS_NAME})

# copy test data
file(GLOB SYSTEM_TESTS_JSON_FILES ${PROJECT_SOURCE_DIR}/system_test/test_json/*)
file(COPY ${SYSTEM_TESTS_JSON_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_json)

file(GLOB SYSTEM_TESTS_IMAGE_FILES ${PROJECT_SOURCE_DIR}/system_test/test_images/*)
file(COPY ${SYSTEM_TESTS_IMAGE_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_images)

file(GLOB SYSTEM_TESTS_IMAGE_FILES ${PROJECT_SOURCE_DIR}/pvpf_algorithm/*)
file(COPY ${SYSTEM_TESTS_IMAGE_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/pvpf_algorithm)
