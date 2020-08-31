cmake_minimum_required(VERSION 3.17)

find_package(yaml-cpp REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(convert_collision convert_collision.cpp)
add_executable(print_ans  print_ans.cpp)
add_executable(print_its  print_its.cpp)
add_executable(parse_ect  parse_ect.cpp)
add_executable(parse_sca  parse_sca.cpp)
add_executable(parse_objtbl  parse_objtbl.cpp)

target_link_libraries(convert_collision
  PRIVATE okami-utils)
target_link_libraries(print_ans
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(print_its 
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_ect
  PRIVATE okami-utils)
target_link_libraries(parse_sca
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_objtbl
  PRIVATE okami-utils
  PRIVATE yaml-cpp)

include_directories(${OPENGL_INCLUDE_DIRS})