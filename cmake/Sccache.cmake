function(learncpp_enable_sccache)
  if(NOT LEARNCPP_USE_SCCACHE)
    return()
  endif()

  find_program(SCCACHE_EXECUTABLE NAMES sccache)
  if(NOT SCCACHE_EXECUTABLE)
    message(STATUS "sccache not found on PATH; building without compiler caching.")
    return()
  endif()

  set(CMAKE_C_COMPILER_LAUNCHER   "${SCCACHE_EXECUTABLE}" PARENT_SCOPE)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${SCCACHE_EXECUTABLE}" PARENT_SCOPE)

  if(MSVC)
    # clang-cl + sccache works best with embedded debug info; the preset enforces it,
    # but reinforce here for sub-projects (e.g. GoogleTest) that override the format.
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "Embedded" PARENT_SCOPE)
  endif()

  message(STATUS "sccache enabled: ${SCCACHE_EXECUTABLE}")
endfunction()
