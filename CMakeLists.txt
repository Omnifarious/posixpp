cmake_minimum_required (VERSION 3.17)

project (posixpp VERSION 0.1.0.0 LANGUAGES CXX)

add_subdirectory(Catch2)
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_LIST_DIR}/Catch2/contrib
)

find_package(fmt REQUIRED)

add_library(posixpp SHARED empty.cpp pubincludes/posixpp/simpleio.h)
set_property(TARGET posixpp PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(posixpp PUBLIC cxx_std_20)
target_include_directories(posixpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/pubincludes>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(posixpp_static STATIC empty.cpp pubincludes/posixpp/simpleio.h)
set_property(TARGET posixpp_static PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(posixpp_static PUBLIC cxx_std_20)
target_include_directories(posixpp_static PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/pubincludes>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

add_executable(all_tests
        tests/simplefd.cpp tests/expected.cpp tests/flagset.cpp
        tests/tempdir.h pubincludes/posixpp/fdflags.h
        pubincludes/pppbase/flagset.h tests/flagset.cpp
        pubincludes/syscalls/linux/x86_64/fdflags.h tests/fdflags.cpp
        pubincludes/syscalls/linux/x86_64/modeflags.h
        pubincludes/posixpp/modeflags.h pubincludes/syscalls/linux/basic.h pubincludes/posixpp/basic.h pubincludes/posixpp/simpleio.h)
set_property(TARGET all_tests PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(all_tests PUBLIC cxx_std_20)
target_link_libraries(all_tests Catch2::Catch2 fmt::fmt posixpp)

add_executable(junk
        tempdevjunk.cpp)
set_property(TARGET junk PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(junk PUBLIC cxx_std_20)
target_link_libraries(junk posixpp)

# This currently only works when optimization can eliminate all exception
# handling code. Exception handling requires C++ runtime support that's not
# yet implemented as part of this library.
add_executable(helloworld
        examples/helloworld.cpp)
set_property(TARGET helloworld PROPERTY CXX_EXTENSIONS OFF)
target_compile_features(helloworld PUBLIC cxx_std_20)
target_link_libraries(helloworld posixpp_static)
# These are specific to g++. I think they will also work with CLang.
target_link_options(helloworld PUBLIC -nodefaultlibs -nostartfiles -e main)

include(CTest)
include(Catch)
catch_discover_tests(all_tests)

add_library(posixpp::posixpp ALIAS posixpp)
