// CRTP routes a base interface to a derived implementation at compile time.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_static_polymorphism
// Topic id : part2/stage06/section06/crtp_static_polymorphism
//
// static_cast is safe here because every derived type names itself in Evaluator<Derived>.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <class Derived>
struct Evaluator {
    constexpr int evaluate(int value) const { return static_cast<const Derived&>(*this).evaluate_impl(value); }
};

struct Square : Evaluator<Square> {
    constexpr int evaluate_impl(int value) const { return value * value; }
};

struct Double : Evaluator<Double> {
    constexpr int evaluate_impl(int value) const { return value * 2; }
};

template <class Derived>
constexpr int evaluate_through_base(const Evaluator<Derived>& evaluator, int value) {
    return evaluator.evaluate(value);
}

static_assert(evaluate_through_base(Square{}, 4) == 16);
static_assert(!std::is_polymorphic_v<Square>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section06/crtp_static_polymorphism"};
    LEARN_EXPECT_EQ(checks, evaluate_through_base(Square{}, 5), 25);
    LEARN_EXPECT_EQ(checks, evaluate_through_base(Double{}, 5), 10);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_static_polymorphism", run>;

}  // namespace
