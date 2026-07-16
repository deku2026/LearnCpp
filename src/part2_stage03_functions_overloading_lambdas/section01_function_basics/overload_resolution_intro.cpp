// Overload resolution ranks implicit conversion sequences.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : overload_resolution_intro
// Topic id : part2/stage03/section01/overload_resolution_intro
//
// Exact matches beat promotions, which beat other standard conversions.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

enum class Selected { integer, floating_point, c_string };

Selected select(int) {
    return Selected::integer;
}

Selected select(double) {
    return Selected::floating_point;
}

Selected select(const char*) {
    return Selected::c_string;
}

using DoubleOverload = Selected (*)(double);
static_assert(std::is_same_v<decltype(static_cast<DoubleOverload>(&select)), DoubleOverload>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section01/overload_resolution_intro"};
    LEARN_EXPECT(checks, select(42) == Selected::integer);
    LEARN_EXPECT(checks, select(3.5) == Selected::floating_point);
    LEARN_EXPECT(checks, select("text") == Selected::c_string);

    // char -> int is an integral promotion and is better than char -> double.
    LEARN_EXPECT(checks, select('A') == Selected::integer);

    // Taking the address of one overload supplies the target type explicitly.
    DoubleOverload floating = static_cast<DoubleOverload>(&select);
    LEARN_EXPECT(checks, floating(7.0) == Selected::floating_point);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/overload_resolution_intro", run>;

}  // namespace
