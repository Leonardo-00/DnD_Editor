cmake_minimum_required(VERSION 2.6)

project(test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
add_subdirectory(lib/googletest)
set(gtest_SOURCE_DIR, lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES runAllTests.cpp ../../src/RuleClass.cpp ../../src/CharClass.cpp
        ./../pugixml-1.10/src/pugixml.cpp
        ../../src/SystemManager.cpp
        ../../src/Head.cpp ../../src/Character.cpp)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(test ${TEST_SOURCE_FILES})
target_link_libraries(test gtest gtest_main)