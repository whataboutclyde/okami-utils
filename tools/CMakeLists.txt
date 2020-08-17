cmake_minimum_required(VERSION 3.17)

#find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(parse_its parse_its.cpp)

target_link_libraries(parse_its 
  PRIVATE okami-utils
  PRIVATE yaml-cpp)
#target_link_libraries(parse_its 
#  PRIVATE okami-utils
#  PRIVATE fmt::fmt
#  PRIVATE yaml-cpp)