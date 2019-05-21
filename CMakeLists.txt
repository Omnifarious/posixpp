cmake_minimum_required (VERSION 3.12)

project (posixpp VERSION 0.1.0.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build libNet as a shared library." ON)

add_library(posixpp empty.cpp)
set_property(TARGET posixpp PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(posixpp PUBLIC cxx_std_17)

target_include_directories(
  posixpp PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
  )
