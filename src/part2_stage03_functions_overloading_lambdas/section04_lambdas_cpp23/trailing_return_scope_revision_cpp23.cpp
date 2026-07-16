// P2036R3 changed name lookup in a lambda trailing return type.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : trailing_return_scope_revision_cpp23
// Topic id : part2/stage03/section04/trailing_return_scope_revision_cpp23
//
// In C++23, the init-capture below is visible in decltype(value).  Previously
// that name found the outer double, silently giving this lambda a double result.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if (defined(LEARNCPP_HAS_P2036_LAMBDA_SCOPE) && LEARNCPP_HAS_P2036_LAMBDA_SCOPE) || \
    (!defined(LEARNCPP_HAS_P2036_LAMBDA_SCOPE) &&                                    \
     ((defined(_MSVC_LANG) && _MSVC_LANG >= 202302L) ||                              \
      (!defined(_MSC_VER) && defined(__cplusplus) && __cplusplus >= 202302L)))
    learn::ExampleChecks checks{"part2/stage03/section04/trailing_return_scope_revision_cpp23"};

    [[maybe_unused]] double value = 1.5;
    const auto captured_type_wins = [value = 7]() -> decltype(value) { return value; };
    static_assert(std::is_same_v<decltype(captured_type_wins()), int>);
    LEARN_EXPECT_EQ(checks, captured_type_wins(), 7);

    const auto text_length = [text = "scope"]() -> decltype(text) { return text; };
    static_assert(std::is_same_v<decltype(text_length()), const char*>);
    LEARN_EXPECT_EQ(checks, text_length()[0], 's');
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section04/trailing_return_scope_revision_cpp23",
                                             "C++23 lambda trailing-return scope (P2036R3)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/trailing_return_scope_revision_cpp23", run>;

}  // namespace
