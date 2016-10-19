cmake_minimum_required(VERSION 3.5)
project(.)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(vector_tests vector_tests.cpp vector3d.cpp)
add_executable(gtest_sample gtest_sample.cpp)

target_link_libraries(vector_tests ${GTEST_LIBRARIES})
target_link_libraries(gtest_sample ${GTEST_LIBRARIES})