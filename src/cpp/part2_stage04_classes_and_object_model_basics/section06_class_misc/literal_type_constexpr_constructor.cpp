// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : literal_type_constexpr_constructor
// Topic id : part2/stage04/section06/literal_type_constexpr_constructor
//
// Covers: constexpr constructor, compile-time objects, literal types

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

struct Point {
    int x = 0;
    int y = 0;
    constexpr Point(int x_, int y_) : x(x_), y(y_) {}
    constexpr int sum() const { return x + y; }
};

struct Scale {
    int factor = 1;
    constexpr explicit Scale(int f) : factor(f) {}
    constexpr int apply(int v) const { return v * factor; }
};

void demo_basics() {
    constexpr Point origin{0, 0};
    static_assert(origin.x == 0);
    assert(origin.y == 0);
}

void demo_intermediate() {
    constexpr Point p{3, 4};
    static_assert(p.sum() == 7);
    assert(p.sum() == 7);
}

void demo_expert() {
    constexpr Scale s{5};
    static_assert(s.apply(2) == 10);
    Point runtime{1, 2};
    assert(runtime.sum() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/literal_type_constexpr_constructor", run>;

}  // namespace
