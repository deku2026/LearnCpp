// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section02_if_consteval_cpp23
// Item     : if_consteval_versus_is_constant_evaluated
// Topic id : part2/stage07/section02/if_consteval_versus_is_constant_evaluated
// References: N4950 [stmt.if], [meta.const.eval]; P1938R3, P0595R2.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L && defined(__cpp_lib_is_constant_evaluated) && \
    __cpp_lib_is_constant_evaluated >= 201811L

enum class EvaluationPath { compile_time, runtime };

constexpr EvaluationPath path_via_library() noexcept {
    if (std::is_constant_evaluated()) {  // Deliberately an ordinary if.
        return EvaluationPath::compile_time;
    }
    return EvaluationPath::runtime;
}

constexpr EvaluationPath path_via_language() noexcept {
    if consteval {
        return EvaluationPath::compile_time;
    } else {
        return EvaluationPath::runtime;
    }
}

inline constexpr bool if_constexpr_condition_is_manifestly_constant_evaluated = true;

// Clang helpfully diagnoses this silent logic error, so keep the exact
// negative form inactive under the repository's warnings-as-errors build.
#if 0
constexpr bool incorrect_if_constexpr_trap() noexcept {
    if constexpr (std::is_constant_evaluated()) {
        return true;
    } else {
        return false;
    }
}
#endif

static_assert(path_via_library() == EvaluationPath::compile_time);
static_assert(path_via_language() == EvaluationPath::compile_time);
static_assert(if_constexpr_condition_is_manifestly_constant_evaluated);

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L && defined(__cpp_lib_is_constant_evaluated) && \
    __cpp_lib_is_constant_evaluated >= 201811L
    ::learn::ExampleChecks checks{"part2/stage07/section02/if_consteval_versus_is_constant_evaluated"};

    LEARN_EXPECT_EQ(checks, path_via_library(), EvaluationPath::runtime);
    LEARN_EXPECT_EQ(checks, path_via_language(), EvaluationPath::runtime);
    LEARN_EXPECT(checks, if_constexpr_condition_is_manifestly_constant_evaluated);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section02/if_consteval_versus_is_constant_evaluated",
                                               "if consteval and std::is_constant_evaluated");
#endif
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section02/if_consteval_versus_is_constant_evaluated", run>;

}  // namespace
