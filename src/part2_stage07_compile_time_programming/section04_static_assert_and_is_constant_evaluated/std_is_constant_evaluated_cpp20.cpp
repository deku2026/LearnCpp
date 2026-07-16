// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : std_is_constant_evaluated_cpp20
// Topic id : part2/stage07/section04/std_is_constant_evaluated_cpp20
// References: N4950 [meta.const.eval], [expr.const]; P0595R2.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

#if defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated >= 201811L

constexpr int context_sensitive_scale(int value) noexcept {
    if (std::is_constant_evaluated()) {  // This must be an ordinary if.
        return value * 10;
    }
    return value * 20;
}

inline constexpr bool if_constexpr_condition_is_manifestly_constant_evaluated = true;

#if 0
// The condition is evaluated in a manifestly constant-evaluated context and
// is therefore always true; Clang diagnoses this useful teaching mistake.
constexpr bool incorrectly_uses_if_constexpr() noexcept {
    if constexpr (std::is_constant_evaluated()) {
        return true;
    } else {
        return false;
    }
}
#endif

static_assert(context_sensitive_scale(2) == 20);
static_assert(if_constexpr_condition_is_manifestly_constant_evaluated);

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated >= 201811L
    ::learn::ExampleChecks checks{"part2/stage07/section04/std_is_constant_evaluated_cpp20"};

    int runtime_value = 2;
    LEARN_EXPECT_EQ(checks, context_sensitive_scale(runtime_value), 40);
    LEARN_EXPECT(checks, if_constexpr_condition_is_manifestly_constant_evaluated);

    constexpr int compile_time_value = context_sensitive_scale(3);
    LEARN_EXPECT_EQ(checks, compile_time_value, 30);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section04/std_is_constant_evaluated_cpp20",
                                               "__cpp_lib_is_constant_evaluated >= 201811L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/std_is_constant_evaluated_cpp20", run>;

}  // namespace
