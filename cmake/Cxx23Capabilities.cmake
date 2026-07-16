include_guard(GLOBAL)

include(CheckCXXSourceCompiles)
include(CMakePushCheckState)

# Run a complete compile-and-link check in the active C++23 toolchain.  Each
# result is cached by CMake inside its own build tree, so strict matrix entries
# must use separate build directories.
function(_learncpp_check_cxx_capability result_variable source)
  cmake_push_check_state(RESET)
  set(CMAKE_REQUIRED_QUIET TRUE)
  if(ARGN)
    set(CMAKE_REQUIRED_LIBRARIES ${ARGN})
  endif()
  check_cxx_source_compiles("${source}" "${result_variable}")
  cmake_pop_check_state()

  set("${result_variable}" "${${result_variable}}" PARENT_SCOPE)
endfunction()

function(_learncpp_capability_as_integer output_variable probe_result)
  if(probe_result)
    set("${output_variable}" 1 PARENT_SCOPE)
  else()
    set("${output_variable}" 0 PARENT_SCOPE)
  endif()
endfunction()

function(learncpp_configure_cxx23_capabilities target_name)
  if(NOT TARGET "${target_name}")
    message(FATAL_ERROR
      "learncpp_configure_cxx23_capabilities requires an existing target; "
      "'${target_name}' does not exist.")
  endif()

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_P2036_LAMBDA_SCOPE
    [=[
      int main() {
        auto captured = [only_in_capture = 42]() -> decltype(only_in_capture) {
          return only_in_capture;
        };
        return captured() == 42 ? 0 : 1;
      }
    ]=]
  )

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_CONSTEXPR_NON_LITERAL_SIGNATURES
    [=[
      struct non_literal {
        int value;
        ~non_literal() {}
      };

      constexpr non_literal round_trip(non_literal value) {
        return value;
      }

      int main() {
        return 0;
      }
    ]=]
  )

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_CONSTEXPR_STRING_PIPELINES
    [=[
      #include <string>

      constexpr bool edit_pipeline() {
        std::string text{"compile"};
        text += " time";
        text.replace(0, 7, "constant");
        return text == "constant time" && text.find("time") == 9;
      }

      constexpr bool dynamic_allocation_pipeline() {
        std::string value;
        value.append(32, 'x');
        value.append(10, 'y');
        value.erase(0, 2);
        return value.size() == 40;
      }

      static_assert(edit_pipeline());
      static_assert(dynamic_allocation_pipeline());

      int main() {
        return 0;
      }
    ]=]
  )

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_REFERENCE_FROM_TEMPORARY_TRAITS
    [=[
      #include <type_traits>

      using constructs =
        std::reference_constructs_from_temporary<const int&, int>;
      using converts =
        std::reference_converts_from_temporary<const int&, int>;

      static_assert(constructs::value == constructs::value);
      static_assert(converts::value == converts::value);

      int main() {
        return 0;
      }
    ]=]
  )

  set(_learncpp_stacktrace_source [=[
    #include <cstddef>
    #include <stacktrace>

    #if !defined(__cpp_lib_stacktrace) || __cpp_lib_stacktrace < 202011L
    #error "The C++23 stacktrace facility is not exposed by this standard library."
    #endif

    std::size_t capture_stack_depth() {
      return std::stacktrace::current(0, 8).size();
    }

    int main() {
      return capture_stack_depth() > 8 ? 1 : 0;
    }
  ]=])

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_STACKTRACE_DEFAULT_LINK
    "${_learncpp_stacktrace_source}"
  )

  set(_learncpp_stacktrace_library "")
  if(NOT _LEARNCPP_PROBE_STACKTRACE_DEFAULT_LINK)
    _learncpp_check_cxx_capability(
      _LEARNCPP_PROBE_STACKTRACE_STDCXXEXP_LINK
      "${_learncpp_stacktrace_source}"
      stdc++exp
    )
    if(_LEARNCPP_PROBE_STACKTRACE_STDCXXEXP_LINK)
      set(_learncpp_stacktrace_library stdc++exp)
    endif()
  endif()

  set(_learncpp_atomic_source [=[
    #include <atomic>

    int main() {
      std::atomic<int> value{0};
      return value.is_lock_free() ? 0 : 1;
    }
  ]=])

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_ATOMIC_DEFAULT_LINK
    "${_learncpp_atomic_source}"
  )

  set(_learncpp_atomic_library "")
  if(NOT _LEARNCPP_PROBE_ATOMIC_DEFAULT_LINK)
    _learncpp_check_cxx_capability(
      _LEARNCPP_PROBE_ATOMIC_LIBRARY_LINK
      "${_learncpp_atomic_source}"
      atomic
    )
    if(_LEARNCPP_PROBE_ATOMIC_LIBRARY_LINK)
      set(_learncpp_atomic_library atomic)
    else()
      message(FATAL_ERROR
        "LearnCpp uses std::atomic::is_lock_free(), but the active toolchain "
        "cannot link it either directly or with libatomic.")
    endif()
  endif()

  _learncpp_capability_as_integer(
    _learncpp_has_p2036_lambda_scope
    "${_LEARNCPP_PROBE_P2036_LAMBDA_SCOPE}"
  )
  _learncpp_capability_as_integer(
    _learncpp_has_constexpr_non_literal_signatures
    "${_LEARNCPP_PROBE_CONSTEXPR_NON_LITERAL_SIGNATURES}"
  )
  _learncpp_capability_as_integer(
    _learncpp_has_constexpr_string_evaluation
    "${_LEARNCPP_PROBE_CONSTEXPR_STRING_PIPELINES}"
  )
  _learncpp_capability_as_integer(
    _learncpp_has_reference_from_temporary_traits
    "${_LEARNCPP_PROBE_REFERENCE_FROM_TEMPORARY_TRAITS}"
  )

  if(_LEARNCPP_PROBE_STACKTRACE_DEFAULT_LINK OR
      _LEARNCPP_PROBE_STACKTRACE_STDCXXEXP_LINK)
    set(_learncpp_has_linkable_stacktrace 1)
  else()
    set(_learncpp_has_linkable_stacktrace 0)
  endif()

  if(_learncpp_stacktrace_library)
    target_link_libraries("${target_name}" PRIVATE "${_learncpp_stacktrace_library}")
  endif()
  if(_learncpp_atomic_library)
    target_link_libraries("${target_name}" PRIVATE "${_learncpp_atomic_library}")
  endif()

  target_compile_definitions("${target_name}" PRIVATE
    LEARNCPP_HAS_P2036_LAMBDA_SCOPE=${_learncpp_has_p2036_lambda_scope}
    LEARNCPP_HAS_CONSTEXPR_NON_LITERAL_SIGNATURES=${_learncpp_has_constexpr_non_literal_signatures}
    LEARNCPP_HAS_CONSTEXPR_STRING_EVALUATION=${_learncpp_has_constexpr_string_evaluation}
    LEARNCPP_HAS_REFERENCE_FROM_TEMPORARY_TRAITS=${_learncpp_has_reference_from_temporary_traits}
    LEARNCPP_HAS_LINKABLE_STACKTRACE=${_learncpp_has_linkable_stacktrace}
  )

  message(STATUS
    "LearnCpp C++23 capabilities: "
    "P2036 lambda scope=${_learncpp_has_p2036_lambda_scope}, "
    "constexpr non-literal signatures=${_learncpp_has_constexpr_non_literal_signatures}, "
    "constexpr string evaluation=${_learncpp_has_constexpr_string_evaluation}, "
    "reference-from-temporary traits=${_learncpp_has_reference_from_temporary_traits}, "
    "linkable stacktrace=${_learncpp_has_linkable_stacktrace}")
  if(_learncpp_stacktrace_library)
    message(STATUS
      "LearnCpp: std::stacktrace requires and will link ${_learncpp_stacktrace_library}.")
  endif()
  if(_learncpp_atomic_library)
    message(STATUS
      "LearnCpp: std::atomic::is_lock_free requires and will link ${_learncpp_atomic_library}.")
  endif()
endfunction()
