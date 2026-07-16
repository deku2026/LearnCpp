// C-style variadic arguments rely on an external type-and-count contract.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section02_variadic_functions
// Item     : legacy_c_variadic
// Topic id : part2/stage03/section02/legacy_c_variadic
//
// The safe call below obeys that contract.  A constrained parameter pack then
// demonstrates the modern type-checked replacement; no mismatched va_arg is run.

#include "learn/example_support.hpp"

#include <concepts>
#include <cstdarg>
#include <cstddef>
#include <type_traits>

namespace {

int legacy_sum(std::size_t count, ...) {
    va_list arguments;
    va_start(arguments, count);

    int result = 0;
    for (std::size_t index = 0; index < count; ++index) {
        // The caller contract for this example says every promoted argument is int.
        result += va_arg(arguments, int);
    }

    va_end(arguments);
    return result;
}

template <std::integral... Values>
    requires(sizeof...(Values) > 0)
constexpr auto checked_sum(Values... values) {
    using Result = std::common_type_t<Values...>;
    return (Result{} + ... + static_cast<Result>(values));
}

static_assert(checked_sum(1, 2L, 3LL) == 6LL);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section02/legacy_c_variadic"};

    const char promoted = 4;
    LEARN_EXPECT_EQ(checks, legacy_sum(4, 1, 2, 3, promoted), 10);

    // Types and arity are available to the compiler in the replacement.
    const auto safe = checked_sum(1, 2L, 3LL, static_cast<short>(4));
    static_assert(std::is_same_v<decltype(safe), const long long>);
    LEARN_EXPECT_EQ(checks, safe, 10LL);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/legacy_c_variadic", run>;

}  // namespace
