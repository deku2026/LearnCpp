// C++23 front attributes can appertain to a lambda's call operator.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : attributes_on_lambda_cpp23
// Topic id : part2/stage03/section04/attributes_on_lambda_cpp23
//
// P2173R1 added the front-attribute position used by [[nodiscard]] below.
// The standard provides no dedicated feature-test macro, so standard mode is gated.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202302L) || \
    (!defined(_MSC_VER) && defined(__cplusplus) && __cplusplus >= 202302L)
    learn::ExampleChecks checks{"part2/stage03/section04/attributes_on_lambda_cpp23"};

    const auto important_result = [] [[nodiscard("use the computed value")]] (int value) { return value * 2; };
    static_assert(std::is_invocable_r_v<int, decltype(important_result), int>);
    LEARN_EXPECT_EQ(checks, important_result(21), 42);

    // An explicit cast documents intentional discard and suppresses nodiscard diagnostics.
    static_cast<void>(important_result(1));
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section04/attributes_on_lambda_cpp23",
                                             "C++23 lambda front attributes (P2173R1)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/attributes_on_lambda_cpp23", run>;

}  // namespace
