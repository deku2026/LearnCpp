// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section02_if_consteval_cpp23
// Item     : if_consteval_calls_consteval_function
// Topic id : part2/stage07/section02/if_consteval_calls_consteval_function
// References: N4950 [stmt.if], [expr.const], [dcl.constexpr]; P1938R3.

#include "learn/example_support.hpp"

namespace {

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L

consteval int immediate_square(int value) {
    if (value < 0) {
        throw "immediate_square requires a non-negative value";
    }
    return value * value;
}

constexpr int square_with_two_paths(int value) {
    if consteval {
        // This substatement is an immediate function context, so a function
        // parameter may be forwarded to a consteval function.
        return immediate_square(value);
    } else {
        return value < 0 ? -1 : value * value;
    }
}

static_assert(square_with_two_paths(6) == 36);

#if 0
// A normal if does not create an immediate function context.
constexpr int rejected(int value) {
    if (std::is_constant_evaluated()) {
        return immediate_square(value);
    }
    return value * value;
}
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    ::learn::ExampleChecks checks{"part2/stage07/section02/if_consteval_calls_consteval_function"};

    constexpr int compile_time_result = square_with_two_paths(7);
    int runtime_argument = 8;
    LEARN_EXPECT_EQ(checks, compile_time_result, 49);
    LEARN_EXPECT_EQ(checks, square_with_two_paths(runtime_argument), 64);
    LEARN_EXPECT_EQ(checks, square_with_two_paths(-1), -1);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section02/if_consteval_calls_consteval_function",
                                               "__cpp_if_consteval >= 202106L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section02/if_consteval_calls_consteval_function", run>;

}  // namespace
