// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : narrowing_check_with_braces
// Topic id : part2/stage02/section01/narrowing_check_with_braces
//

#include "learn/example_support.hpp"

#include <concepts>
#include <utility>

namespace {

template <class Target, class Source>
concept BraceInitializableFrom = requires(Source&& source) { Target{std::forward<Source>(source)}; };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(BraceInitializableFrom<int, int>);
    static_assert(!BraceInitializableFrom<int, double>);

    const double fractional = 3.75;
    const int parenthesized(fractional);  // Allowed, but discards the fraction.
    const int braced{3};                  // Exact integral source is safe.

    learn::ExampleChecks checks{"part2/stage02/section01/narrowing_check_with_braces"};
    LEARN_EXPECT_EQ(checks, parenthesized, 3);
    LEARN_EXPECT_EQ(checks, braced, 3);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/narrowing_check_with_braces", run>;

}  // namespace
