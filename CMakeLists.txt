cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

option(LEARNCPP_ENABLE_ASAN "Enable address sanitizer." OFF)
option(LEARNCPP_ENABLE_UBSAN "Enable undefined-behavior sanitizer (clang/gcc only)." OFF)
option(LEARNCPP_ENABLE_CLANG_TIDY "Run clang-tidy during the build." OFF)
option(LEARNCPP_WERROR "Treat warnings as errors." OFF)
option(LEARNCPP_USE_SCCACHE "Use sccache as the compiler launcher when available." ON)

project(LearnCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This project intentionally uses headers rather than C++ modules. Avoid the
# module dependency scan for every translation unit in this large target.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND DEFINED ENV{INCLUDE})
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES $ENV{INCLUDE})
  set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES $ENV{INCLUDE})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Sccache)
include(CompileCommands)
include(CompilerWarnings)
include(Cxx23Capabilities)
include(Sanitizers)
include(StaticAnalysis)

learncpp_enable_sccache()

find_package(Threads REQUIRED)

# Every topic lives under src/<part>_<stage>/<section>/<item>.cpp and registers
# itself through ::learn::topic<Id, run>. GLOB_RECURSE picks up new files between builds.
file(GLOB_RECURSE LEARNCPP_SOURCES
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE LEARNCPP_HEADERS
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${CMAKE_SOURCE_DIR}/include/*.hpp"
  "${CMAKE_SOURCE_DIR}/include/*.h"
)

list(LENGTH LEARNCPP_SOURCES LEARNCPP_SOURCE_COUNT)
message(STATUS "LearnCpp: collected ${LEARNCPP_SOURCE_COUNT} translation units under src/.")

add_executable(learn_cpp
  ${LEARNCPP_SOURCES}
  ${LEARNCPP_HEADERS}
)

target_compile_features(learn_cpp PRIVATE cxx_std_23)
target_include_directories(learn_cpp PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(learn_cpp PRIVATE Threads::Threads)
if(CMAKE_DL_LIBS)
  target_link_libraries(learn_cpp PRIVATE ${CMAKE_DL_LIBS})
endif()

if(MSVC)
  # Topic metadata contains UTF-8 Chinese document names. Make the source and
  # execution character sets explicit so cl.exe /WX builds do not emit C4819.
  target_compile_options(learn_cpp PRIVATE /utf-8)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # CMake's Clang frontend-only -std flag does not update clang-cl's
    # _MSVC_LANG compatibility macro. MSVC STL keys several C++23 paths from
    # that macro, so use the driver option as well. This is also the mode used
    # by the repository's direct VS build and local topic checks.
    target_compile_options(learn_cpp PRIVATE /std:c++latest)
  endif()

  # Each topic is a self-registering translation unit. MSVC's default Release
  # /OPT:REF can discard those otherwise-unreferenced registration COMDATs and
  # leave an empty registry, so retaining them is part of the executable model.
  target_link_options(learn_cpp PRIVATE /OPT:NOREF)
endif()
set_target_properties(learn_cpp PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
learncpp_apply_warning_settings(learn_cpp)
learncpp_enable_sanitizers(learn_cpp)
learncpp_configure_static_analysis(learn_cpp)
learncpp_configure_cxx23_capabilities(learn_cpp)

source_group(TREE "${CMAKE_SOURCE_DIR}/src"
  PREFIX "Topics"
  FILES ${LEARNCPP_SOURCES})
source_group(TREE "${CMAKE_SOURCE_DIR}/include"
  PREFIX "Headers"
  FILES ${LEARNCPP_HEADERS})

learncpp_enable_compile_commands_sync()
