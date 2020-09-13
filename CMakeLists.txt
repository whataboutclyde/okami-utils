cmake_minimum_required(VERSION 3.17)

project(okami-utils VERSION 0.1
  DESCRIPTION "Utilities for parsing Okami HD PC files."
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(OKAMI_CPP_BUILD_TOOLS "Enable parse tools" ON)

find_package(fmt REQUIRED)
find_package(OpenGL REQUIRED)

add_library(okami-utils STATIC 
  src/common.cpp include/okami-utils/common.h
  src/ak.cpp  include/okami-utils/ak.h
  src/akt.cpp include/okami-utils/akt.h
  src/ans.cpp include/okami-utils/ans.h
  src/archive.cpp include/okami-utils/archive.h
  src/ect.cpp include/okami-utils/ect.h
  src/its.cpp include/okami-utils/its.h
  src/mesh_division.cpp include/okami-utils/mesh_division.h
  src/model.cpp include/okami-utils/model.h
  src/objtbl.cpp include/okami-utils/objtbl.h
  src/submesh.cpp include/okami-utils/submesh.h
  src/zone.cpp include/okami-utils/zone.h)
target_include_directories(okami-utils PUBLIC include)
target_link_libraries(okami-utils
  PRIVATE fmt)

include_directories(${OPENGL_INCLUDE_DIRS})

if(OKAMI_CPP_BUILD_TOOLS)
	add_subdirectory(tools)
  add_subdirectory(gltf)
endif()