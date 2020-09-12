cmake_minimum_required(VERSION 3.15)
project(DnD_Editor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

#Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(pugixml-1.10)

add_subdirectory(test)

add_executable(DnD_Editor src/main.cpp src/RuleClass.cpp src/CharClass.cpp pugixml-1.10/src/pugixml.cpp src/SystemManager.cpp
        src/Head.cpp src/Character.cpp src/Character.h)