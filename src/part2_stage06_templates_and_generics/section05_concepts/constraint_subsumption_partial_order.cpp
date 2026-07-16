// Constraint subsumption lets a more constrained overload win partial ordering.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : constraint_subsumption_partial_order
// Topic id : part2/stage06/section05/constraint_subsumption_partial_order
//
// Reusing the named Number concept gives the compiler identical atomic constraints.

#include "learn/example_support.hpp"

#include <concepts>

namespace {

template <class T>
concept Number = std::integral<T> || std::floating_point<T>;

template <class T>
concept IntegralNumber = Number<T> && std::integral<T>;

enum class SelectedConstraint { number, integral_number };

template <Number T>
constexpr SelectedConstraint select_constraint(T) {
    return SelectedConstraint::number;
}

template <IntegralNumber T>
constexpr SelectedConstraint select_constraint(T) {
    return SelectedConstraint::integral_number;
}

static_assert(select_constraint(1) == SelectedConstraint::integral_number);
static_assert(select_constraint(1.5) == SelectedConstraint::number);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section05/constraint_subsumption_partial_order"};
    LEARN_EXPECT(checks, select_constraint(7) == SelectedConstraint::integral_number);
    LEARN_EXPECT(checks, select_constraint(7.0) == SelectedConstraint::number);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/constraint_subsumption_partial_order", run>;

}  // namespace
