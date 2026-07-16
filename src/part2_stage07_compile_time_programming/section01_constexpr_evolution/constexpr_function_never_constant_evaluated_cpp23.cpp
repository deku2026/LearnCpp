// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_function_never_constant_evaluated_cpp23
// Topic id : part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23
// References: N4950 [dcl.constexpr]; P2448R2.

#include "learn/example_support.hpp"

namespace {

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L

int runtime_increment(int value) noexcept {
    return value + 1;
}

// P2448 removed the old ill-formed-no-diagnostic-required rule. This
// declaration is valid even though every invocation calls a non-constexpr
// function and therefore no invocation can be a constant expression.
constexpr int runtime_adapter(int value) noexcept {
    return runtime_increment(value);
}

// A context that actually requires a constant expression still diagnoses it.
#if 0
constexpr int rejected = runtime_adapter(41);
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    ::learn::ExampleChecks checks{"part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23"};

    LEARN_EXPECT_EQ(checks, runtime_adapter(41), 42);
    LEARN_EXPECT_EQ(checks, runtime_adapter(-1), 0);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(
        "part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23",
        "__cpp_constexpr >= 202211L (P2448)");
#endif
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23", run>;

}  // namespace
