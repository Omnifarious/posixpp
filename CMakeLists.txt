cmake_minimum_required (VERSION 3.12)

project (posixpp VERSION 0.1.0.0 LANGUAGES CXX)

find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)

option(BUILD_SHARED_LIBS "Build libNet as a shared library." ON)

add_library(posixpp empty.cpp)
set_property(TARGET posixpp PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(posixpp PUBLIC cxx_std_17)

add_executable(mytest mytest.cpp)
set_property(TARGET mytest PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(mytest PUBLIC cxx_std_17)
target_link_libraries(mytest Catch2::Catch2 fmt::fmt)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(mytest)

target_include_directories(
  posixpp PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/pubincludes"
  )
target_include_directories(
  mytest PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/pubincludes"
  )
