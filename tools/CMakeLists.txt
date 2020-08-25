cmake_minimum_required(VERSION 3.17)

find_package(yaml-cpp REQUIRED)

add_executable(akt_to_obj akt_to_obj.cpp)
add_executable(parse_ans  parse_ans.cpp)
add_executable(parse_its  parse_its.cpp)
add_executable(parse_sca  parse_sca.cpp)
add_executable(parse_objtbl  parse_objtbl.cpp)

target_link_libraries(akt_to_obj
  PRIVATE okami-utils)
target_link_libraries(parse_ans
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_its 
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_sca
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_objtbl
  PRIVATE okami-utils
  PRIVATE yaml-cpp)