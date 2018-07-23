cmake_minimum_required(VERSION 3.10)
project(Assignments)

set(CMAKE_CXX_STANDARD 11)
set(OUTPUT_DIR ../bin/)

add_executable(A0
        0/image.cpp
        0/image.h
        0/matrix.cpp
        0/matrix.h
        0/vectors.h
        0/ifs.cpp
        0/ifs.h
        0/main.cpp)

set_target_properties(A0 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A0"
        OUTPUT_NAME "ifs")

