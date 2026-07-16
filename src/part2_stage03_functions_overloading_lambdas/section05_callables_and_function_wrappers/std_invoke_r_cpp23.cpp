// C++23 invoke_r requests a specific result type from an invocation.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_invoke_r_cpp23
// Topic id : part2/stage03/section05/std_invoke_r_cpp23
//
// The R constraint is checked through is_invocable_r; R=void deliberately
// discards the callable's result while preserving its side effects.

#include "learn/example_support.hpp"

#include <functional>
#include <type_traits>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    learn::ExampleChecks checks{"part2/stage03/section05/std_invoke_r_cpp23"};

    const auto half = [](int value) -> double { return value / 2.0; };
    static_assert(std::is_invocable_r_v<int, decltype(half), int>);
    static_assert(std::is_invocable_r_v<void, decltype(half), int>);
    LEARN_EXPECT_EQ(checks, std::invoke_r<int>(half, 7), 3);
    LEARN_EXPECT_EQ(checks, std::invoke_r<double>(half, 7), 3.5);

    int calls = 0;
    auto side_effect = [&calls] {
        ++calls;
        return 99;
    };
    std::invoke_r<void>(side_effect);
    LEARN_EXPECT_EQ(checks, calls, 1);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section05/std_invoke_r_cpp23",
                                             "std::invoke_r (__cpp_lib_invoke_r >= 202106L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_invoke_r_cpp23", run>;

}  // namespace
