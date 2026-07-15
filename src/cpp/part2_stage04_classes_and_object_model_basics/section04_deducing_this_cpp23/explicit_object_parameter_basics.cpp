// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : explicit_object_parameter_basics
// Topic id : part2/stage04/section04/explicit_object_parameter_basics
//
// Covers: C++23 explicit object parameter (deducing this) basics

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

#if defined(__cpp_explicit_this_parameter)

struct Counter {
    int n = 0;

    void inc(this Counter& self) { ++self.n; }
    int get(this const Counter& self) { return self.n; }
};

void demo_basics() {
    Counter c;
    c.inc();
    c.inc();
    assert(c.get() == 2);
}

void demo_intermediate() {
    const Counter c{5};
    assert(c.get() == 5);
    // c.inc(); // would not bind non-const explicit object param
}

void demo_expert() {
    // Call syntax stays obj.method(); object is the explicit first parameter.
    Counter c;
    c.inc();
    assert((&c)->get() == 1);
}

#else

void demo_basics() {
    // Feature unavailable: safe no-op path.
    assert(true);
}

void demo_intermediate() {
    assert(true);
}

void demo_expert() {
#if !defined(__cpp_explicit_this_parameter)
    constexpr bool available = false;
#else
    constexpr bool available = true;
#endif
    assert(!available);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/explicit_object_parameter_basics", run>;

}  // namespace
