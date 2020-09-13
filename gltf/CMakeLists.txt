cmake_minimum_required(VERSION 3.17)

find_package(OpenGL REQUIRED)

add_executable(generate_gltf 
  generate_gltf.cpp
  gltf.cpp gltf.h
  wrappers/gltf_akt.cpp wrappers/gltf_akt.h
  wrappers/gltf_zone.cpp wrappers/gltf_zone.h)

add_executable(parse_model
  parse_model.cpp
  gltf.cpp gltf.h)

target_link_libraries(generate_gltf
  PRIVATE okami-utils)
target_link_libraries(parse_model
  PRIVATE okami-utils)

include_directories(${OPENGL_INCLUDE_DIRS})