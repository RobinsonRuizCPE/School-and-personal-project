#CMakeLists.txt to build project using cmake
#run $cmake CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

project(cpe_surface)
FIND_PACKAGE(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -O2 ")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(
  GLOB_RECURSE 
  source_files
  project/*.cc project/*.h
)

add_executable(
  cpe_surface
  ${source_files}
)

target_link_libraries(cpe_surface ${OPENGL_LIBRARIES} -L/usr/lib/nvidia-384 -lGL -lGLU -lglut -lGLEW )
