cmake_minimum_required(VERSION 3.17)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(okami-utils VERSION 0.1
                    DESCRIPTION "Utilities for parsing Okami HD PC files."
                    LANGUAGES CXX)

add_library(okami-utils STATIC 
            src/ans.cpp include/okami-utils/ans.h
            src/its.cpp include/okami-utils/its.h
            src/sca.cpp include/okami-utils/sca.h)
target_include_directories(okami-utils PUBLIC include)