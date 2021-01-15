cmake_minimum_required(VERSION 3.17)

find_package(OpenGL REQUIRED)

add_executable(generate_gltf 
  generate_gltf.cpp
  gltf.cpp gltf.h
  wrappers/gltf_akt.cpp wrappers/gltf_akt.h
  wrappers/gltf_zone.cpp wrappers/gltf_zone.h)

target_link_libraries(generate_gltf
  PRIVATE okami-utils)

include_directories(${OPENGL_INCLUDE_DIRS})