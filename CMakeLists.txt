cmake_minimum_required(VERSION 3.17)

project(okami-utils VERSION 0.1
  DESCRIPTION "Utilities for parsing Okami HD PC files."
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(OKAMI_CPP_BUILD_TOOLS "Enable parse tools" ON)

add_library(okami-utils STATIC 
  src/ans.cpp include/okami-utils/ans.h
  src/its.cpp include/okami-utils/its.h
  src/sca.cpp include/okami-utils/sca.h)
target_include_directories(okami-utils PUBLIC include)

if(OKAMI_CPP_BUILD_TOOLS)
	add_subdirectory(tools)
endif()