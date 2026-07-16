// A requires-clause attaches a constraint to a template declaration.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : requires_clause
// Topic id : part2/stage06/section05/requires_clause
//
// Leading and trailing positions are equivalent ways to make the contract visible.

#include "learn/example_support.hpp"

#include <concepts>
#include <string>

namespace {

template <class T>
    requires std::totally_ordered<T>
constexpr T smaller(T left, T right) {
    return right < left ? right : left;
}

template <class T>
constexpr T larger(T left, T right)
    requires std::totally_ordered<T>
{
    return left < right ? right : left;
}

template <class T>
concept HasOrderedHelpers = requires(T value) {
    smaller(value, value);
    larger(value, value);
};

struct NotOrdered {};

static_assert(HasOrderedHelpers<int>);
static_assert(!HasOrderedHelpers<NotOrdered>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section05/requires_clause"};
    LEARN_EXPECT_EQ(checks, smaller(7, 3), 3);
    LEARN_EXPECT_EQ(checks, larger(7, 3), 7);
    LEARN_EXPECT_EQ(checks, smaller(std::string{"beta"}, std::string{"alpha"}), std::string{"alpha"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/requires_clause", run>;

}  // namespace
