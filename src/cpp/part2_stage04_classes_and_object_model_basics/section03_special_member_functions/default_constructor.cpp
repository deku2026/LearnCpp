// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : default_constructor
// Topic id : part2/stage04/section03/default_constructor
//
// Covers: implicit default ctor, =default, suppressed when any ctor is declared

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct Implicit {
    int x = 0;
};

struct ExplicitDefault {
    int x = 1;
    ExplicitDefault() = default;
};

struct NoDefault {
    int x;
    explicit NoDefault(int v) : x(v) {}
};

void demo_basics() {
    Implicit a;
    assert(a.x == 0);

    ExplicitDefault b;
    assert(b.x == 1);

    NoDefault c{5};
    assert(c.x == 5);
}

void demo_intermediate() {
    static_assert(std::is_default_constructible_v<Implicit>);
    static_assert(std::is_default_constructible_v<ExplicitDefault>);
    static_assert(!std::is_default_constructible_v<NoDefault>);
}

void demo_expert() {
    // Declaring any constructor suppresses the implicit default constructor.
    // Request it back with = default when needed.
    struct Both {
        int x = 0;
        Both() = default;
        explicit Both(int v) : x(v) {}
    };
    Both d;
    Both e{9};
    assert(d.x == 0 && e.x == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/default_constructor", run>;

}  // namespace
