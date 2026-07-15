// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : static_member
// Topic id : part2/stage04/section06/static_member
//
// Covers: inline static data, static member functions, shared state

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

class Counter {
    static inline int count_ = 0;
    int id_ = 0;

public:
    Counter() : id_(++count_) {}
    static int total() { return count_; }
    int id() const { return id_; }
    static void reset_for_demo() { count_ = 0; }
};

void demo_basics() {
    Counter::reset_for_demo();
    Counter a;
    Counter b;
    assert(Counter::total() == 2);
    assert(a.id() == 1);
    assert(b.id() == 2);
}

void demo_intermediate() {
    Counter::reset_for_demo();
    assert(Counter::total() == 0);
    Counter c;
    assert(c.id() == 1);
    assert(Counter::total() == 1);
}

void demo_expert() {
    // Static member function has no this; call via type or object.
    Counter::reset_for_demo();
    Counter c;
    assert(c.total() == 1);
    assert(Counter::total() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/static_member", run>;

}  // namespace
