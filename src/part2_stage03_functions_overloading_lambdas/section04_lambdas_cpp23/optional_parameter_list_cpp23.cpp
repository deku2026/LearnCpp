// P1102R2 lets a parameterless C++23 lambda omit () before specifiers.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : optional_parameter_list_cpp23
// Topic id : part2/stage03/section04/optional_parameter_list_cpp23
//
// There is no dedicated SD-6 macro for this grammar change, so the example is
// enabled only in an advertised C++23 language mode.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202302L) || \
    (!defined(_MSC_VER) && defined(__cplusplus) && __cplusplus >= 202302L)
    learn::ExampleChecks checks{"part2/stage03/section04/optional_parameter_list_cpp23"};

    auto counter = [value = 0] mutable { return ++value; };
    constexpr auto answer = [] constexpr noexcept -> int { return 42; };
    static_assert(noexcept(answer()));
    static_assert(std::is_invocable_r_v<int, decltype(counter)&>);
    static_assert(answer() == 42);

    LEARN_EXPECT_EQ(checks, counter(), 1);
    LEARN_EXPECT_EQ(checks, counter(), 2);
    LEARN_EXPECT_EQ(checks, answer(), 42);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section04/optional_parameter_list_cpp23",
                                             "C++23 optional lambda parameter lists (P1102R2)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/optional_parameter_list_cpp23", run>;

}  // namespace
