cmake_minimum_required (VERSION 3.17)

project (posixpp VERSION 0.1.0.0 LANGUAGES CXX)

add_subdirectory(Catch2)
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_LIST_DIR}/Catch2/contrib
)

find_package(fmt REQUIRED)

option(BUILD_SHARED_LIBS "Build posixpp as a shared library." ON)

add_library(posixpp empty.cpp)
set_property(TARGET posixpp PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(posixpp PUBLIC cxx_std_20)
target_include_directories(posixpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/pubincludes>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_executable(all_tests
        tests/simplefd.cpp tests/expected.cpp tests/flagset.cpp
        tests/tempdir.h pubincludes/posixpp/fdflags.h
        pubincludes/pppbase/flagset.h tests/flagset.cpp
        pubincludes/syscalls/linux/x86_64/fdflags.h tests/fdflags.cpp)
set_property(TARGET all_tests PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(all_tests PUBLIC cxx_std_20)
target_link_libraries(all_tests Catch2::Catch2 fmt::fmt posixpp)

include(CTest)
include(Catch)
catch_discover_tests(all_tests)

add_library(posixpp::posixpp ALIAS posixpp)
