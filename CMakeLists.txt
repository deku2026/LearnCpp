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

if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND DEFINED ENV{INCLUDE})
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES $ENV{INCLUDE})
  set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES $ENV{INCLUDE})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Sccache)
include(CompileCommands)
include(CompilerWarnings)
include(Sanitizers)
include(StaticAnalysis)

learncpp_enable_sccache()

# Every C++ topic lives under src/cpp/<part>_<stage>/<section>/<item>.cpp and
# registers itself through learn::topic<...>. GLOB_RECURSE picks up new files.
# Kotlin lives under src/kotlin/ (Gradle sourceSets); never compiled by CMake.
# Native FFM shared library lives under src/cpp/ffm/ and is a separate target.
file(GLOB_RECURSE LEARNCPP_SOURCES
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${CMAKE_SOURCE_DIR}/src/cpp/*.cpp"
)
# Exclude shared-library sources from the topic executable.
list(FILTER LEARNCPP_SOURCES EXCLUDE REGEX "[/\\\\]ffm[/\\\\]")

file(GLOB_RECURSE LEARNCPP_HEADERS
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${CMAKE_SOURCE_DIR}/include/*.hpp"
  "${CMAKE_SOURCE_DIR}/include/*.h"
)

list(LENGTH LEARNCPP_SOURCES LEARNCPP_SOURCE_COUNT)
message(STATUS "LearnCpp: collected ${LEARNCPP_SOURCE_COUNT} translation units under src/cpp.")

add_executable(learn_cpp
  ${LEARNCPP_SOURCES}
  ${LEARNCPP_HEADERS}
)

target_compile_features(learn_cpp PRIVATE cxx_std_23)
target_include_directories(learn_cpp PRIVATE "${CMAKE_SOURCE_DIR}/include")
set_target_properties(learn_cpp PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
learncpp_apply_warning_settings(learn_cpp)
learncpp_enable_sanitizers(learn_cpp)
learncpp_configure_static_analysis(learn_cpp)

source_group(TREE "${CMAKE_SOURCE_DIR}/src/cpp"
  PREFIX "Topics"
  FILES ${LEARNCPP_SOURCES})
source_group(TREE "${CMAKE_SOURCE_DIR}/include"
  PREFIX "Headers"
  FILES ${LEARNCPP_HEADERS})

# learn_ffm: C ABI shared lib for Java FFM (JEP 454). Output: build/<preset>/bin/
add_library(learn_ffm SHARED
  "${CMAKE_SOURCE_DIR}/src/cpp/ffm/learn_ffm.cpp"
  "${CMAKE_SOURCE_DIR}/src/cpp/ffm/learn_ffm.h"
)
target_compile_features(learn_ffm PRIVATE cxx_std_23)
target_include_directories(learn_ffm PUBLIC "${CMAKE_SOURCE_DIR}/src/cpp/ffm")
target_compile_definitions(learn_ffm PRIVATE LEARN_FFM_BUILD)
set_target_properties(learn_ffm PROPERTIES
  OUTPUT_NAME "learn_ffm"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
learncpp_apply_warning_settings(learn_ffm)

source_group(TREE "${CMAKE_SOURCE_DIR}/src/cpp/ffm"
  PREFIX "FFM"
  FILES
    "${CMAKE_SOURCE_DIR}/src/cpp/ffm/learn_ffm.cpp"
    "${CMAKE_SOURCE_DIR}/src/cpp/ffm/learn_ffm.h"
)

learncpp_enable_compile_commands_sync()
