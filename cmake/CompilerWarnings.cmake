function(learncpp_apply_warning_settings target_name)
  if(MSVC)
    set(learncpp_warning_flags /W4 /EHsc "$<$<COMPILE_LANGUAGE:CXX>:/permissive->")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      # clang-cl driver: tame MSVC-flavored noise without losing real warnings.
      list(APPEND learncpp_warning_flags
        -Wno-unused-command-line-argument
        -Wno-unused-parameter
        -Wno-microsoft-include
      )
    else()
      list(APPEND learncpp_warning_flags /external:anglebrackets /external:W0 /wd4100)
    endif()

    if(LEARNCPP_WERROR)
      list(APPEND learncpp_warning_flags /WX)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(learncpp_warning_flags
      -Wall
      -Wextra
      -Wpedantic
      -Wshadow
      -Wuninitialized
      -Wno-unused-parameter
    )
    if(LEARNCPP_WERROR)
      list(APPEND learncpp_warning_flags -Werror)
    endif()
  else()
    set(learncpp_warning_flags)
  endif()

  if(learncpp_warning_flags)
    target_compile_options(${target_name} PRIVATE ${learncpp_warning_flags})
  endif()
endfunction()
