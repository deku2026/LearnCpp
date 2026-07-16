// A requires-expression checks simple, type, compound, and nested requirements.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : requires_expression
// Topic id : part2/stage06/section05/requires_expression
//
// The expression is unevaluated and becomes false when a dependent requirement is invalid.

#include "learn/example_support.hpp"

#include <concepts>
#include <cstddef>
#include <string>
#include <vector>

namespace {

template <class T>
concept Sequence = requires(T& sequence, const T& const_sequence) {
    typename T::value_type;
    const_sequence.size();
    { sequence.front() } -> std::same_as<typename T::value_type&>;
    requires std::copyable<T>;
};

template <Sequence T>
auto first_value(const T& sequence) -> typename T::value_type {
    return sequence.front();
}

struct NoSequenceInterface {};

static_assert(Sequence<std::vector<int>>);
static_assert(Sequence<std::string>);
static_assert(!Sequence<NoSequenceInterface>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section05/requires_expression"};
    LEARN_EXPECT_EQ(checks, first_value(std::vector<int>{4, 5}), 4);
    LEARN_EXPECT_EQ(checks, first_value(std::string{"requirements"}), 'r');
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/requires_expression", run>;

}  // namespace
