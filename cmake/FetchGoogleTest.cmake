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

  # GoogleTest bakes /WX into its own targets, which turns clang-cl 22's new
  # -Wcharacter-conversion (char8_t -> char32_t inside gtest-printers.h) into a
  # build break. Suppress just that one warning on the gtest targets; we don't
  # touch global warning settings.
  foreach(_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
    if(TARGET ${_gtest_target})
      if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${_gtest_target} PRIVATE
          -Wno-character-conversion
          -Wno-unused-command-line-argument
        )
      elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${_gtest_target} PRIVATE
          -Wno-character-conversion
        )
      endif()
    endif()
  endforeach()
endfunction()
