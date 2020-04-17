set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_FLAGS "-g" )

cmake_minimum_required (VERSION 2.8.11)
project (OpenGLRenderer)
set(SOURCES
    ./src/main.cpp
    ./src/opengldriver/opengldriver.cpp
    ./src/opengldriver/rendermanager.cpp
    ./src/opengldriver/opengl/glprogram.cpp
    ./src/opengldriver/opengl/glframebuffer.cpp
    ./src/opengldriver/opengl/gbufferfbo.cpp
    ./src/opengldriver/opengl3/glpipeline.cpp
    ./src/opengldriver/job/job.cpp
    ./src/opengldriver/job/directlightingjob.cpp
    ./src/opengldriver/job/gbufferjob.cpp
    ./src/opengldriver/job/skyboxjob.cpp
    ./src/opengldriver/job/framebufferjob.cpp
    ./src/opengldriver/job/ambientocclusionjob.cpp
    ./src/renderer/renderer.cpp
    ./src/renderer/mesh.cpp
    ./src/renderer/camera.cpp
    ./src/renderer/mat4.cpp
    ./src/util/util.cpp
    )
set(DIRS
    ./src
    ./src/3rdparty
    ./src/opengldriver
    ./src/opengldriver/opengl
    ./src/opengldriver/opengl3
    ./src/opengldriver/opengl/job
    ./src/renderer
    ./src/renderer/materials
    ./src/renderer/scene
    ./src/util
    )
add_executable(OpenGLRenderer ${SOURCES})
include_directories(${DIRS})
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(OpenGLRenderer ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} SDL2)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
