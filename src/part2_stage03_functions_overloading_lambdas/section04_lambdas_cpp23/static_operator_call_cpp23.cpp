// P1169R4 permits a captureless C++23 lambda to have static operator().
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : static_operator_call_cpp23
// Topic id : part2/stage03/section04/static_operator_call_cpp23
//
// A static call operator has no implicit object parameter.  The standard
// feature-test macro also prevents unsupported parsers from seeing the syntax.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
    learn::ExampleChecks checks{"part2/stage03/section04/static_operator_call_cpp23"};

    constexpr auto square = [](int value) static noexcept { return value * value; };
    static_assert(square(5) == 25);
    static_assert(noexcept(square(1)));
    static_assert(std::is_empty_v<decltype(square)>);
    static_assert(std::is_convertible_v<decltype(square), int (*)(int) noexcept>);

    int (*function_pointer)(int) noexcept = square;
    LEARN_EXPECT_EQ(checks, square(6), 36);
    LEARN_EXPECT_EQ(checks, function_pointer(7), 49);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section04/static_operator_call_cpp23",
                                             "static operator() (__cpp_static_call_operator >= 202207L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/static_operator_call_cpp23", run>;

}  // namespace
