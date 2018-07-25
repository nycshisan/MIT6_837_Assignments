cmake_minimum_required(VERSION 3.10)
project(Assignments)

set(CMAKE_CXX_STANDARD 11)
set(OUTPUT_DIR ../bin/)

add_executable(A0
        src/0/image.cpp
        src/0/image.h
        src/0/matrix.cpp
        src/0/matrix.h
        src/0/vectors.h
        src/0/ifs.cpp
        src/0/ifs.h
        src/0/main.cpp)

set_target_properties(A0 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A0"
        OUTPUT_NAME "ifs")

add_executable(A1
        src/1/main.cpp
        src/1/matrix.cpp
        src/1/image.cpp
        src/1/scene_parser.cpp
        src/1/sphere.cpp
        src/1/raytracer.cpp
        src/1/camera.cpp
        src/1/group.cpp)

set_target_properties(A1 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A1"
        OUTPUT_NAME "raytracer")

add_executable(A2
        src/2/main.cpp
        src/2/matrix.cpp
        src/2/image.cpp
        src/2/scene_parser.cpp
        src/2/sphere.cpp
        src/2/raytracer.cpp
        src/2/camera.cpp
        src/2/group.cpp
        src/2/plane.cpp
        src/2/triangle.cpp
        src/2/transform.cpp
        src/2/commandLineArgumentParser.cpp)

set_target_properties(A2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A2"
        OUTPUT_NAME "raytracer")

add_executable(A3
        src/3/main.cpp
        src/3/matrix.cpp
        src/3/image.cpp
        src/3/sphere.cpp
        src/3/raytracer.cpp
        src/3/camera.cpp
        src/3/group.cpp
        src/3/plane.cpp
        src/3/triangle.cpp
        src/3/transform.cpp
        src/3/commandLineArgumentParser.cpp
        src/3/light.cpp
        src/3/glCanvas.cpp)

set_target_properties(A3 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A3"
        OUTPUT_NAME "raytracer")

target_link_libraries(A3 OpenGL GLUT)