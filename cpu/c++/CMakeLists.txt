cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(BASE_LINE)

set(CMAKE_CXX_FLAGS "-w -g -std=c++17 -lpthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
# set(CMAKE_CXX_FLAGS_RELEASE "-g -Wall")

include(ExternalProject)
ExternalProject_Add(
    google_gtest
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${BASE_LINE_SOURCE_DIR}/third/gtest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${BASE_LINE_SOURCE_DIR}/third/gtest/build -DBUILD_SHARED_LIBS=OFF
)

ExternalProject_Add(
    boostorg_boost
    URL https://jaist.dl.sourceforge.net/project/boost/boost/1.72.0/boost_1_72_0.7z
    PREFIX ${BASE_LINE_SOURCE_DIR}/third/boost
    BUILD_IN_SOURCE true
    CONFIGURE_COMMAND ""
    BUILD_COMMAND sh bootstrap.sh && ./b2 link=static -j8
    INSTALL_COMMAND ./b2 install --prefix=${HELLO_CPP_SOURCE_DIR}/third/boost/build
)

include_directories(
    "${BASE_LINE_SOURCE_DIR}/include"
    "${BASE_LINE_SOURCE_DIR}/third/gtest/build/include"
    "${BASE_LINE_SOURCE_DIR}/third/boost/build/include"
    "/usr/local/Cellar/boost/1.72.0_2/include"
)

link_directories(
    "${BASE_LINE_SOURCE_DIR}/third/gtest/build/lib"
    "${BASE_LINE_SOURCE_DIR}/third/boost/build/lib"
    "/usr/local/Cellar/boost/1.72.0_2/include/lib"
)

aux_source_directory(src/ source)
add_executable(baseline ${source})
# add_dependencies(baseline boostorg_boost)

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

