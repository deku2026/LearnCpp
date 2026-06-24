cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

option(LEARNCPP_ENABLE_ASAN "Enable address sanitizer." OFF)
option(LEARNCPP_ENABLE_UBSAN "Enable undefined-behavior sanitizer (clang/gcc only)." OFF)
option(LEARNCPP_ENABLE_CLANG_TIDY "Run clang-tidy during the build." OFF)
option(LEARNCPP_WERROR "Treat warnings as errors." OFF)
option(LEARNCPP_USE_SCCACHE "Use sccache as the compiler launcher when available." ON)
option(LEARNCPP_BUILD_TESTS "Build the GoogleTest-driven exercise harness." ON)

project(LearnCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

if(LEARNCPP_BUILD_TESTS)
  enable_testing()
  include(FetchGoogleTest)
  learncpp_fetch_googletest()
endif()

# Every exercise lives under src/<stage>/<topic>/<small_item>.cpp.
# GLOB_RECURSE + CONFIGURE_DEPENDS keeps Ninja honest about new files,
# while still letting us add placeholders by just dropping a .cpp.
file(GLOB_RECURSE LEARNCPP_EXERCISE_SOURCES
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE LEARNCPP_EXERCISE_HEADERS
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${CMAKE_SOURCE_DIR}/src/*.h"
  "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

list(LENGTH LEARNCPP_EXERCISE_SOURCES LEARNCPP_EXERCISE_SOURCE_COUNT)
message(STATUS "LearnCpp: collected ${LEARNCPP_EXERCISE_SOURCE_COUNT} exercise translation units under src/.")

if(LEARNCPP_BUILD_TESTS)
  add_executable(learn_cpp_tests
    ${LEARNCPP_EXERCISE_SOURCES}
    ${LEARNCPP_EXERCISE_HEADERS}
  )

  target_compile_features(learn_cpp_tests PRIVATE cxx_std_23)
  target_include_directories(learn_cpp_tests PRIVATE "${CMAKE_SOURCE_DIR}/src")
  target_link_libraries(learn_cpp_tests PRIVATE GTest::gtest GTest::gtest_main)
  set_target_properties(learn_cpp_tests
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )
  learncpp_apply_warning_settings(learn_cpp_tests)
  learncpp_enable_sanitizers(learn_cpp_tests)
  learncpp_configure_static_analysis(learn_cpp_tests)

  source_group(TREE "${CMAKE_SOURCE_DIR}/src"
    PREFIX "Exercises"
    FILES ${LEARNCPP_EXERCISE_SOURCES} ${LEARNCPP_EXERCISE_HEADERS})

  include(GoogleTest)
  gtest_discover_tests(learn_cpp_tests
    PROPERTIES
      LABELS "LearnCpp"
      TIMEOUT 60
  )
endif()

learncpp_enable_compile_commands_sync()
