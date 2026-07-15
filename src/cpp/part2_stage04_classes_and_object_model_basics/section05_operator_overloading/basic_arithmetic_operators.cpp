// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : basic_arithmetic_operators
// Topic id : part2/stage04/section05/basic_arithmetic_operators
//
// Covers: member + and +=, non-member symmetry notes

#include "learn/topic_registry.hpp"

namespace {

class Vec2 {
    double x_ = 0;
    double y_ = 0;

public:
    Vec2() = default;
    Vec2(double x, double y) : x_(x), y_(y) {}

    Vec2 operator+(const Vec2& rhs) const { return {x_ + rhs.x_, y_ + rhs.y_}; }
    Vec2& operator+=(const Vec2& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    double x() const { return x_; }
    double y() const { return y_; }
};

void demo_basics() {
    Vec2 a{1, 2};
    Vec2 b{3, 4};
    Vec2 c = a + b;
    LEARN_CHECK(c.x() == 4 && c.y() == 6);
}

void demo_intermediate() {
    Vec2 a{1, 1};
    a += Vec2{2, 3};
    LEARN_CHECK(a.x() == 3 && a.y() == 4);
}

void demo_expert() {
    Vec2 a{1, 0};
    Vec2 b = a + a + a;
    LEARN_CHECK(b.x() == 3 && b.y() == 0);
    (a += Vec2{1, 1}) += Vec2{1, 1};
    LEARN_CHECK(a.x() == 3 && a.y() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/basic_arithmetic_operators", run>;

}  // namespace
