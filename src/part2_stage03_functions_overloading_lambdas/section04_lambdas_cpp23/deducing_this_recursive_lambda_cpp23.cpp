// C++23 explicit object parameters let a lambda name and call itself.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : deducing_this_recursive_lambda_cpp23
// Topic id : part2/stage03/section04/deducing_this_recursive_lambda_cpp23
//
// This avoids std::function type erasure and the older extra-self-argument
// call syntax.  P0847R7 is detected by its standard feature-test macro.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    learn::ExampleChecks checks{"part2/stage03/section04/deducing_this_recursive_lambda_cpp23"};

    constexpr auto factorial = [](this auto&& self, unsigned value) -> unsigned long long {
        return value < 2 ? 1ULL : value * self(value - 1);
    };
    constexpr auto fibonacci = [](this const auto& self, unsigned value) -> unsigned {
        return value < 2 ? value : self(value - 1) + self(value - 2);
    };

    static_assert(factorial(6) == 720ULL);
    static_assert(fibonacci(10) == 55U);
    static_assert(std::is_invocable_r_v<unsigned long long, decltype(factorial), unsigned>);
    LEARN_EXPECT_EQ(checks, factorial(8), 40'320ULL);
    LEARN_EXPECT_EQ(checks, fibonacci(12), 144U);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section04/deducing_this_recursive_lambda_cpp23",
                                             "explicit object parameters (__cpp_explicit_this_parameter >= 202110L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/deducing_this_recursive_lambda_cpp23", run>;

}  // namespace
