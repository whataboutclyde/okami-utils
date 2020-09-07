cmake_minimum_required(VERSION 3.17)

find_package(yaml-cpp REQUIRED)

add_executable(print_ans  print_ans.cpp)
add_executable(print_its  print_its.cpp)
add_executable(parse_ect  parse_ect.cpp)
add_executable(parse_zones  parse_zones.cpp)
add_executable(parse_objtbl  parse_objtbl.cpp)
add_executable(pack_akt pack_akt.cpp)
add_executable(unpack_akt unpack_akt.cpp)
add_executable(unpack_event unpack_event.cpp)

target_link_libraries(print_ans
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(print_its 
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_ect
  PRIVATE okami-utils)
target_link_libraries(parse_zones
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_objtbl
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(unpack_event
  PRIVATE okami-utils)