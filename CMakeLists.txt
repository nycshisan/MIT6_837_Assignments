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

add_executable(A1
        1/main.cpp
        1/matrix.cpp
        1/image.cpp
        1/scene_parser.cpp
        1/sphere.cpp
        1/raytracer.cpp
        1/camera.cpp
        1/group.cpp)

set_target_properties(A1 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A1"
        OUTPUT_NAME "raytracer")

add_executable(A2
        2/main.cpp
        2/matrix.cpp
        2/image.cpp
        2/scene_parser.cpp
        2/sphere.cpp
        2/raytracer.cpp
        2/camera.cpp
        2/group.cpp
        2/plane.cpp
        2/triangle.cpp
        2/transform.cpp
        2/commandLineArgumentParser.cpp)

set_target_properties(A2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A2"
        OUTPUT_NAME "raytracer")
