# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.16)
project ("ProtoCAD")

add_subdirectory("dependencies")
add_subdirectory("source")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")