cmake_minimum_required (VERSION 3.17)

project (posixpp VERSION 0.1.0.0 LANGUAGES CXX)

find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)

option(BUILD_SHARED_LIBS "Build libNet as a shared library." ON)

add_library(posixpp empty.cpp)
set_property(TARGET posixpp PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(posixpp PUBLIC cxx_std_20)

add_executable(all_tests tests/simplefd.cpp tests/expected.cpp tests/tempdir.h)
set_property(TARGET all_tests PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(all_tests PUBLIC cxx_std_20)
target_link_libraries(all_tests Catch2::Catch2 fmt::fmt posixpp)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(all_tests)

target_include_directories(
  posixpp PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/pubincludes"
  )
target_include_directories(
  all_tests PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/pubincludes"
  )
