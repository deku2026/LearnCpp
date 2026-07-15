// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : explicit_constructor
// Topic id : part2/stage04/section01/explicit_constructor
//
// Covers: explicit single-arg ctors, implicit conversion traps, explicit conversion ops intro

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct Buffer {
    int size = 0;
    explicit Buffer(int n) : size(n) {}
};

struct Loose {
    int size = 0;
    Loose(int n) : size(n) {}
};

int use_loose(Loose l) {
    return l.size;
}

int use_buffer(Buffer b) {
    return b.size;
}

void demo_basics() {
    Buffer b{10};
    assert(b.size == 10);
    assert(use_buffer(Buffer{7}) == 7);

    Loose l = 5;
    assert(l.size == 5);
    assert(use_loose(3) == 3);
}

void demo_intermediate() {
    // explicit blocks copy-init / function-arg conversion from int.
    static_assert(!std::is_convertible_v<int, Buffer>);
    static_assert(std::is_convertible_v<int, Loose>);
    static_assert(std::is_constructible_v<Buffer, int>);
}

void demo_expert() {
    // Prefer explicit on single-arg ctors unless conversion is intentional.
    Buffer b = Buffer{42};
    assert(use_buffer(b) == 42);

    Loose loose = 9;
    assert(use_loose(loose) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/explicit_constructor", run>;

}  // namespace
