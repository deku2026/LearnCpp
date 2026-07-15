// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : explicit_conversion_operators
// Topic id : part2/stage04/section05/explicit_conversion_operators
//
// Covers: explicit operator T, contextual bool conversions

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

class Fraction {
    int num_ = 0;
    int den_ = 1;

public:
    Fraction(int n, int d) : num_(n), den_(d) {}

    explicit operator double() const { return static_cast<double>(num_) / static_cast<double>(den_); }
    explicit operator bool() const { return num_ != 0; }
};

void demo_basics() {
    Fraction f{1, 2};
    double d = static_cast<double>(f);
    LEARN_CHECK(d == 0.5);
}

void demo_intermediate() {
    Fraction z{0, 1};
    Fraction n{3, 4};
    LEARN_CHECK(!static_cast<bool>(z));
    LEARN_CHECK(static_cast<bool>(n));
    if (n) {
        LEARN_CHECK(true);
    }
    if (!z) {
        LEARN_CHECK(true);
    }
}

void demo_expert() {
    static_assert(!std::is_convertible_v<Fraction, double>);
    static_assert(std::is_constructible_v<double, Fraction>);
    Fraction f{2, 5};
    LEARN_CHECK(static_cast<double>(f) == 0.4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/explicit_conversion_operators", run>;

}  // namespace
