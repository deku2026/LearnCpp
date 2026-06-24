include_guard(GLOBAL)
include(FetchContent)

function(learncpp_fetch_googletest)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

  # GoogleTest v1.17.0 (2025) — modern, C++17/20-clean, supports MSVC + clang-cl.
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(googletest)

  # GoogleTest bakes /WX into its own targets, which makes warnings fatal.
  # clang-cl 22 added -Wcharacter-conversion (char8_t -> char32_t inside
  # gtest-printers.h is a hard error there), but older clang/clang-cl on CI
  # doesn't know that warning name and would itself error on
  # -Wunknown-warning-option. Put -Wno-unknown-warning-option first so the
  # newer -Wno-character-conversion is harmless on every version we ship to.
  foreach(_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
    if(TARGET ${_gtest_target})
      if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${_gtest_target} PRIVATE
          -Wno-unknown-warning-option
          -Wno-character-conversion
          -Wno-unused-command-line-argument
        )
      elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${_gtest_target} PRIVATE
          -Wno-unknown-warning-option
          -Wno-character-conversion
        )
      endif()
    endif()
  endforeach()
endfunction()
