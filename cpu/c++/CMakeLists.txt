cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(BASE_LINE)

set(CMAKE_CXX_FLAGS "-w -g -std=c++17 -lpthread")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-g -Wall")

include(ExternalProject)
ExternalProject_Add(
    google_gtest
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${BASE_LINE_SOURCE_DIR}/third/gtest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${BASE_LINE_SOURCE_DIR}/third/gtest/build -DBUILD_SHARED_LIBS=OFF
)

include_directories(
    "${BASE_LINE_SOURCE_DIR}/include"
    "${BASE_LINE_SOURCE_DIR}/third/gtest/build/include"
)

link_directories(
    "${BASE_LINE_SOURCE_DIR}/third/gtest/build/lib"
)

aux_source_directory(src/ source)
add_executable(baseline ${source})

add_custom_target(
    run
    DEPENDS baseline
    COMMAND ./baseline
)

enable_testing()
add_executable(test_big_int test/big_int_test.cpp)
target_link_libraries(test_big_int gtest)
add_dependencies(test_big_int google_gtest)
add_test(NAME test_big_int COMMAND ./test_big_int)

