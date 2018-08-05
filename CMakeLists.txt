cmake_minimum_required(VERSION 3.10)
project(Assignments)

set(CMAKE_CXX_STANDARD 11)
set(OUTPUT_DIR ../bin/)

### A0 ###

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

### A1 ###

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

### A2 ###

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

find_package(OpenGL)
find_package(GLUT)

### A3 ###

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
        src/3/scene_parser.cpp
        src/3/light.cpp
        src/3/glCanvas.cpp
        src/3/material.cpp)

set_target_properties(A3 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A3"
        OUTPUT_NAME "raytracer")

target_link_libraries(A3 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

### A4 ###

add_executable(A4
        src/4/main.cpp
        src/4/matrix.cpp
        src/4/image.cpp
        src/4/sphere.cpp
        src/4/raytracer.cpp
        src/4/camera.cpp
        src/4/group.cpp
        src/4/plane.cpp
        src/4/triangle.cpp
        src/4/transform.cpp
        src/4/commandLineArgumentParser.cpp
        src/4/scene_parser.cpp
        src/4/light.cpp
        src/4/glCanvas.cpp
        src/4/material.cpp
        src/4/rayTree.cpp)

set_target_properties(A4 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A4"
        OUTPUT_NAME "raytracer")

target_link_libraries(A4 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

target_compile_options(A4 PUBLIC "-Wno-deprecated-declarations")

### A5 ###

add_executable(A5
        src/5/main.cpp
        src/5/matrix.cpp
        src/5/image.cpp
        src/5/sphere.cpp
        src/5/raytracer.cpp
        src/5/camera.cpp
        src/5/group.cpp
        src/5/plane.cpp
        src/5/triangle.cpp
        src/5/transform.cpp
        src/5/commandLineArgumentParser.cpp
        src/5/scene_parser.cpp
        src/5/light.cpp
        src/5/glCanvas.cpp
        src/5/material.cpp
        src/5/rayTree.cpp
        src/5/grid.cpp
        src/5/boundingbox.cpp
        src/5/object3d.cpp
        src/5/hsl.cpp)

set_target_properties(A5 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A5"
        OUTPUT_NAME "raytracer")

target_link_libraries(A5 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

target_compile_options(A5 PUBLIC "-Wno-deprecated-declarations")

### A6 ###

add_executable(A6
        src/6/main.cpp
        src/6/matrix.cpp
        src/6/image.cpp
        src/6/sphere.cpp
        src/6/raytracer.cpp
        src/6/camera.cpp
        src/6/group.cpp
        src/6/plane.cpp
        src/6/triangle.cpp
        src/6/transform.cpp
        src/6/commandLineArgumentParser.cpp
        src/6/scene_parser.cpp
        src/6/light.cpp
        src/6/glCanvas.cpp
        src/6/material.cpp
        src/6/rayTree.cpp
        src/6/grid.cpp
        src/6/boundingbox.cpp
        src/6/object3d.cpp
        src/6/hsl.cpp
        src/6/perlin_noise.cpp
        src/6/raytracing_stats.cpp)

set_target_properties(A6 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A6"
        OUTPUT_NAME "raytracer")

target_link_libraries(A6 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

target_compile_options(A6 PUBLIC "-Wno-deprecated-declarations")

### A7 ###

add_executable(A7
        src/7/main.cpp
        src/7/matrix.cpp
        src/7/image.cpp
        src/7/sphere.cpp
        src/7/raytracer.cpp
        src/7/camera.cpp
        src/7/group.cpp
        src/7/plane.cpp
        src/7/triangle.cpp
        src/7/transform.cpp
        src/7/commandLineArgumentParser.cpp
        src/7/scene_parser.cpp
        src/7/light.cpp
        src/7/glCanvas.cpp
        src/7/material.cpp
        src/7/rayTree.cpp
        src/7/grid.cpp
        src/7/boundingbox.cpp
        src/7/object3d.cpp
        src/7/hsl.cpp
        src/7/perlin_noise.cpp
        src/7/raytracing_stats.cpp
        src/7/film.cpp
        src/7/filter.cpp
        src/7/sampler.cpp)

set_target_properties(A7 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A7"
        OUTPUT_NAME "raytracer")

target_link_libraries(A7 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

target_compile_options(A7 PUBLIC "-Wno-deprecated-declarations")

### A8 ###

add_executable(A8
        src/8/main.cpp
        src/8/glCanvas.cpp
        src/8/matrix.cpp
        src/8/spline_parser.cpp
        src/8/triangle_mesh.cpp
        src/8/curve.cpp
        src/8/surface.cpp
        src/8/spline.cpp)

set_target_properties(A8 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A8"
        OUTPUT_NAME "curve_editor")

target_link_libraries(A8 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

target_compile_options(A8 PUBLIC "-Wno-deprecated-declarations")

### A9 ###

add_executable(A9
        src/9/main.cpp
        src/9/glCanvas.cpp
        src/9/matrix.cpp
        src/9/parser.cpp
        src/9/particle.cpp
        src/9/system.cpp
        src/9/generator.cpp
        src/9/integrator.cpp
        src/9/forcefield.cpp)

set_target_properties(A9 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/A9"
        OUTPUT_NAME "particle_system")

target_link_libraries(A9 ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

target_compile_options(A9 PUBLIC "-Wno-deprecated-declarations")