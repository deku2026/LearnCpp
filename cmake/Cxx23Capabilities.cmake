include_guard(GLOBAL)

include(CheckCXXSourceCompiles)
include(CMakePushCheckState)

# Run a complete compile-and-link check in the active C++23 toolchain.  Each
# result is cached by CMake inside its own build tree, so strict matrix entries
# must use separate build directories.
function(_learncpp_check_cxx_capability result_variable source)
  cmake_push_check_state(RESET)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  set(CMAKE_REQUIRED_QUIET TRUE)
  if(_learncpp_required_compile_flags)
    set(CMAKE_REQUIRED_FLAGS "${_learncpp_required_compile_flags}")
  endif()
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
    _LEARNCPP_PROBE_FORMAT_RANGES_PIPELINES
    [=[
      #include <format>
      #include <string>
      #include <tuple>
      #include <vector>
      #include <version>

      #if !defined(__cpp_lib_format) || __cpp_lib_format < 201907L
      #error "The base C++20 formatting facility is not advertised."
      #endif
      #if !defined(__cpp_lib_format_ranges) || __cpp_lib_format_ranges < 202207L
      #error "C++23 range formatting is not advertised."
      #endif

      int main() {
        const std::vector<int> values{1, 2, 3};
        const std::tuple record{"Ada", 23};
        const std::vector<std::vector<int>> matrix{{1, 2}, {3, 4}};
        const std::string formatted_values = std::format("{}", values);
        const std::string padded_values = std::format("{::02}", values);
        const std::string formatted_record = std::format("{}", record);
        const std::string formatted_matrix = std::format("{}", matrix);
        const std::string formatted_empty = std::format("{}", std::vector<int>{});
        return formatted_values.empty() || padded_values.empty() ||
                       formatted_record.empty() || formatted_matrix.empty() ||
                       formatted_empty.empty()
                   ? 1
                   : 0;
      }
    ]=]
  )

  # LLVM issue #101614: Clang can recognize std::forward_like as a builtin
  # without instantiating the deduced return type used by some libstdc++
  # headers.  Probe the real operation instead of keying the workaround to
  # compiler or library versions.  The retry keeps every native C++23 path
  # enabled; it does not suppress a diagnostic.
  set(_learncpp_forward_like_source [=[
    #include <concepts>
    #include <utility>
    #include <version>

    int main() {
    #if defined(__cpp_lib_forward_like) && __cpp_lib_forward_like >= 202207L
      int value = 4;
      static_assert(
        std::same_as<decltype(std::forward_like<int&>(value)), int&>);
      static_assert(
        std::same_as<decltype(std::forward_like<const int&>(value)),
                     const int&>);
      static_assert(
        std::same_as<decltype(std::forward_like<int&&>(value)), int&&>);

      std::forward_like<int&>(value) = 8;
      const int& read_only = std::forward_like<const int&>(value);
      int&& moved = std::forward_like<int&&>(value);
      return value == 8 && read_only == 8 && &moved == &value ? 0 : 1;
    #else
      return 0;
    #endif
    }
  ]=])

  set(_learncpp_clang_forward_like_workaround 0)
  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_FORWARD_LIKE_CALLS
    "${_learncpp_forward_like_source}"
  )
  if(NOT _LEARNCPP_PROBE_FORWARD_LIKE_CALLS AND
      CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
    set(_learncpp_required_compile_flags
      "-fno-builtin-std-forward_like")
    _learncpp_check_cxx_capability(
      _LEARNCPP_PROBE_FORWARD_LIKE_CALLS_WITHOUT_BUILTIN
      "${_learncpp_forward_like_source}"
    )
    if(_LEARNCPP_PROBE_FORWARD_LIKE_CALLS_WITHOUT_BUILTIN)
      set(_learncpp_clang_forward_like_workaround 1)
    else()
      set(_learncpp_required_compile_flags "")
    endif()
  endif()

  if(NOT _LEARNCPP_PROBE_FORWARD_LIKE_CALLS AND
      NOT _LEARNCPP_PROBE_FORWARD_LIKE_CALLS_WITHOUT_BUILTIN)
    message(FATAL_ERROR
      "The active standard library advertises std::forward_like, but the "
      "compiler cannot instantiate it.  Clang's targeted "
      "-fno-builtin-std-forward_like retry also failed when applicable.")
  endif()

  # Validate all library pipelines that transitively instantiate
  # std::forward_like.  Each branch is conditional on the standard feature
  # macro, so an implementation that has not advertised a feature is allowed;
  # an advertised-but-broken implementation fails during configuration.
  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_BIND_FRONT_BACK_CALLS
    [=[
      #include <functional>
      #include <string>
      #include <string_view>
      #include <type_traits>
      #include <version>

      int combine(int first, int second, int third) {
        return first * 100 + second * 10 + third;
      }

      std::string surround(
          std::string_view text,
          std::string_view left,
          std::string_view right) {
        return std::string{left} + std::string{text} + std::string{right};
      }

      struct ledger {
        int balance;
        int deposit(int amount) {
          balance += amount;
          return balance;
        }
      };

      int main() {
      #if defined(__cpp_lib_bind_front) && __cpp_lib_bind_front >= 201907L
        auto front = std::bind_front(combine, 1, 2);
        const auto const_front = std::bind_front(combine, 1, 2);
        static_assert(
          std::is_invocable_r_v<int, decltype(front)&, int>);
        static_assert(
          std::is_invocable_r_v<int, const decltype(front)&, int>);
        if (front(3) != 123 || const_front(3) != 123) {
          return 1;
        }

        ledger account{100};
        auto deposit =
          std::bind_front(&ledger::deposit, std::ref(account));
        if (deposit(25) != 125 || account.balance != 125) {
          return 2;
        }
      #endif

      #if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
        auto back = std::bind_back(combine, 2, 3);
        const auto const_back = std::bind_back(combine, 2, 3);
        static_assert(
          std::is_invocable_r_v<int, decltype(back)&, int>);
        static_assert(
          std::is_invocable_r_v<int, const decltype(back)&, int>);
        if (back(1) != 123 || const_back(1) != 123) {
          return 3;
        }

        auto close_bracket =
          std::bind_back(surround, std::string_view{"]"});
        if (close_bracket(
              std::string_view{"value"}, std::string_view{"["}) !=
            std::string{"[value]"}) {
          return 4;
        }
      #endif
        return 0;
      }
    ]=]
  )
  if(NOT _LEARNCPP_PROBE_BIND_FRONT_BACK_CALLS)
    message(FATAL_ERROR
      "The active standard library advertises bind_front/bind_back, but "
      "their real invocation pipelines do not compile and link.")
  endif()

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_RANGE_ADAPTOR_CLOSURE_COMPOSITION
    [=[
      #include <ranges>
      #include <utility>
      #include <vector>
      #include <version>

      #if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
      struct multiply_by
          : std::ranges::range_adaptor_closure<multiply_by> {
        explicit multiply_by(int value) : factor(value) {}

        int factor;

        template <std::ranges::viewable_range range_type>
        auto operator()(range_type&& range) const {
          return std::forward<range_type>(range) |
                 std::views::transform(
                   [factor = factor](int value) {
                     return value * factor;
                   });
        }
      };

      struct add
          : std::ranges::range_adaptor_closure<add> {
        explicit add(int value) : increment(value) {}

        int increment;

        template <std::ranges::viewable_range range_type>
        auto operator()(range_type&& range) const {
          return std::forward<range_type>(range) |
                 std::views::transform(
                   [increment = increment](int value) {
                     return value + increment;
                   });
        }
      };
      #endif

      int main() {
      #if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
        std::vector values{1, 2, 3};
        const auto times_three = multiply_by{3};
        const auto first_two =
          times_three | std::views::take(2);
        const auto first_pipeline = values | first_two;

        const auto composed = multiply_by{3} | add{-1};
        const std::vector const_values{1, 2, 3};
        const auto second_pipeline = const_values | composed;
        return *first_pipeline.begin() == 3 &&
                       *second_pipeline.begin() == 2
                   ? 0
                   : 1;
      #else
        return 0;
      #endif
      }
    ]=]
  )
  if(NOT _LEARNCPP_PROBE_RANGE_ADAPTOR_CLOSURE_COMPOSITION)
    message(FATAL_ERROR
      "The active standard library advertises C++23 range adaptor closure "
      "composition, but real custom/standard compositions do not compile "
      "and link.")
  endif()

  _learncpp_check_cxx_capability(
    _LEARNCPP_PROBE_RANGES_TO_CONTAINER_PIPE
    [=[
      #include <ranges>
      #include <list>
      #include <set>
      #include <vector>
      #include <version>

      int main() {
      #if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
        std::vector source{1, 2, 2, 3, 4};
        auto even_squares =
          source |
          std::views::filter(
            [](int value) { return value % 2 == 0; }) |
          std::views::transform(
            [](int value) { return value * value; });

        const auto vector_result =
          even_squares | std::ranges::to<std::vector>();
        const auto set_result =
          even_squares | std::ranges::to<std::set>();
        const auto list_result =
          std::ranges::to<std::list<int>>(even_squares);
        return vector_result == std::vector{4, 4, 16} &&
                       set_result == std::set{4, 16} &&
                       list_result.size() == 3 &&
                       list_result.front() == 4 &&
                       list_result.back() == 16
                   ? 0
                   : 1;
      #else
        return 0;
      #endif
      }
    ]=]
  )
  if(NOT _LEARNCPP_PROBE_RANGES_TO_CONTAINER_PIPE)
    message(FATAL_ERROR
      "The active standard library advertises ranges::to, but its vector "
      "and set pipe forms or direct list form do not compile and link.")
  endif()

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
    _learncpp_has_format_ranges
    "${_LEARNCPP_PROBE_FORMAT_RANGES_PIPELINES}"
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
  if(_learncpp_clang_forward_like_workaround)
    target_compile_options("${target_name}" PRIVATE
      -fno-builtin-std-forward_like)
  endif()

  target_compile_definitions("${target_name}" PRIVATE
    LEARNCPP_HAS_P2036_LAMBDA_SCOPE=${_learncpp_has_p2036_lambda_scope}
    LEARNCPP_HAS_CONSTEXPR_NON_LITERAL_SIGNATURES=${_learncpp_has_constexpr_non_literal_signatures}
    LEARNCPP_HAS_CONSTEXPR_STRING_EVALUATION=${_learncpp_has_constexpr_string_evaluation}
    LEARNCPP_HAS_FORMAT_RANGES=${_learncpp_has_format_ranges}
    LEARNCPP_HAS_REFERENCE_FROM_TEMPORARY_TRAITS=${_learncpp_has_reference_from_temporary_traits}
    LEARNCPP_HAS_LINKABLE_STACKTRACE=${_learncpp_has_linkable_stacktrace}
  )

  message(STATUS
    "LearnCpp C++23 capabilities: "
    "P2036 lambda scope=${_learncpp_has_p2036_lambda_scope}, "
    "constexpr non-literal signatures=${_learncpp_has_constexpr_non_literal_signatures}, "
    "constexpr string evaluation=${_learncpp_has_constexpr_string_evaluation}, "
    "format ranges=${_learncpp_has_format_ranges}, "
    "Clang forward_like builtin workaround=${_learncpp_clang_forward_like_workaround}, "
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
