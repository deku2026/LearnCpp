function(learncpp_enable_sanitizers target_name)
  if(LEARNCPP_ENABLE_ASAN)
    if(MSVC)
      target_compile_options(${target_name} PRIVATE /fsanitize=address)

      # CMake drives lld-link directly for clang-cl, so the compiler driver
      # cannot add compiler-rt automatically. Reproduce clang-cl's /MD link
      # contract explicitly and copy the runtime DLL beside the executable.
      # MSVC's link.exe infers the same libraries from instrumented objects.
      if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
          message(FATAL_ERROR
            "clang-cl AddressSanitizer does not support the Debug CRT in this toolchain. "
            "Configure an ASan tree with CMAKE_BUILD_TYPE=RelWithDebInfo instead.")
        endif()

        execute_process(
          COMMAND "${CMAKE_CXX_COMPILER}" --print-resource-dir
          RESULT_VARIABLE _learncpp_clang_resource_result
          OUTPUT_VARIABLE _learncpp_clang_resource_dir
          ERROR_VARIABLE _learncpp_clang_resource_error
          OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(NOT _learncpp_clang_resource_result EQUAL 0)
          message(FATAL_ERROR
            "Unable to locate the clang-cl runtime directory: ${_learncpp_clang_resource_error}")
        endif()

        if(NOT CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "x64")
          message(FATAL_ERROR
            "The LearnCpp clang-cl AddressSanitizer preset currently supports x64 only; "
            "detected '${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}'.")
        endif()
        set(_learncpp_asan_arch "x86_64")
        set(_learncpp_asan_dir "${_learncpp_clang_resource_dir}/lib/windows")
        set(_learncpp_asan_import
          "${_learncpp_asan_dir}/clang_rt.asan_dynamic-${_learncpp_asan_arch}.lib")
        set(_learncpp_asan_thunk
          "${_learncpp_asan_dir}/clang_rt.asan_dynamic_runtime_thunk-${_learncpp_asan_arch}.lib")
        set(_learncpp_asan_dll
          "${_learncpp_asan_dir}/clang_rt.asan_dynamic-${_learncpp_asan_arch}.dll")
        foreach(_learncpp_asan_file IN ITEMS
            "${_learncpp_asan_import}" "${_learncpp_asan_thunk}" "${_learncpp_asan_dll}")
          if(NOT EXISTS "${_learncpp_asan_file}")
            message(FATAL_ERROR "Required clang-cl AddressSanitizer runtime not found: ${_learncpp_asan_file}")
          endif()
        endforeach()

        target_link_libraries(${target_name} PRIVATE "${_learncpp_asan_import}")
        target_link_options(${target_name} PRIVATE
          /INCREMENTAL:NO
          /INCLUDE:__asan_seh_interceptor
          "/WHOLEARCHIVE:${_learncpp_asan_thunk}"
        )
        add_custom_command(TARGET ${target_name} POST_BUILD
          COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${_learncpp_asan_dll}" "$<TARGET_FILE_DIR:${target_name}>"
          VERBATIM
        )
      endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      target_compile_options(${target_name} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
      target_link_options(${target_name} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    else()
      message(WARNING "LEARNCPP_ENABLE_ASAN is only supported on MSVC, Clang, and GCC.")
    endif()
  endif()

  if(LEARNCPP_ENABLE_UBSAN)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      if(NOT MSVC)
        target_compile_options(${target_name} PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
        target_link_options(${target_name} PRIVATE -fsanitize=undefined)
      else()
        message(STATUS "UBSan disabled under clang-cl/MSVC; enable in the Linux/macOS Clang pipelines instead.")
      endif()
    else()
      message(WARNING "LEARNCPP_ENABLE_UBSAN is only supported on Clang/GCC.")
    endif()
  endif()
endfunction()
