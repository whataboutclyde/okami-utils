cmake_minimum_required(VERSION 3.17)

find_package(yaml-cpp REQUIRED)

add_executable(parse_ans  parse_ans.cpp)
add_executable(parse_its  parse_its.cpp)
add_executable(parse_sca  parse_sca.cpp)
add_executable(pack_akt   pack_akt.cpp)
add_executable(unpack_akt unpack_akt.cpp)

target_link_libraries(parse_its 
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_ans
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(parse_sca
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
target_link_libraries(pack_akt
  PRIVATE okami-utils)
target_link_libraries(unpack_akt
  PRIVATE okami-utils)