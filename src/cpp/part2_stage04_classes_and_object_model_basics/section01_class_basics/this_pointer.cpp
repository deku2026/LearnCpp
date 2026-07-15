// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : this_pointer
// Topic id : part2/stage04/section01/this_pointer
//
// Covers: this as implicit object pointer, disambiguation, fluent return *this

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

class Builder {
    int x_ = 0;
    int y_ = 0;

public:
    Builder& set_x(int x) {
        this->x_ = x;
        return *this;
    }

    Builder& set_y(int y) {
        this->y_ = y;
        return *this;
    }

    int x() const { return x_; }
    int y() const { return y_; }

    bool same_as(const Builder* other) const { return this == other; }
};

void demo_basics() {
    Builder b;
    b.set_x(1).set_y(2);
    assert(b.x() == 1 && b.y() == 2);
}

void demo_intermediate() {
    Builder b;
    assert(b.same_as(&b));
    Builder other;
    assert(!b.same_as(&other));
}

void demo_expert() {
    // this is a prvalue of type T* (or const T* in const members).
    // Explicit this-> is mainly for shadowing and returning *this for chaining.
    Builder b;
    Builder& again = b.set_x(9);
    assert(&again == &b);
    assert(b.x() == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/this_pointer", run>;

}  // namespace
