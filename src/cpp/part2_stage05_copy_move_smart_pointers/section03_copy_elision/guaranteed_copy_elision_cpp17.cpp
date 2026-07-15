// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : guaranteed_copy_elision_cpp17
// Topic id : part2/stage05/section03/guaranteed_copy_elision_cpp17
//
// Covers: C++17 guaranteed copy elision for prvalues; deleted move still OK

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct Immobile {
    int value;

    explicit Immobile(int v) : value(v) {}
    Immobile(const Immobile&) = delete;
    Immobile(Immobile&&) = delete;
    Immobile& operator=(const Immobile&) = delete;
    Immobile& operator=(Immobile&&) = delete;
};

Immobile make_immobile() {
    return Immobile{123};
}

void demo_basics() {
    Immobile x = make_immobile();  // guaranteed elision: no move/copy needed
    assert(x.value == 123);
}

void demo_intermediate() {
    Immobile y = Immobile{7};  // prvalue initialization of same type
    assert(y.value == 7);

    auto factory = [] { return Immobile{9}; };
    Immobile z = factory();
    assert(z.value == 9);
}

void demo_expert() {
    static_assert(!std::is_move_constructible_v<Immobile>);
    static_assert(!std::is_copy_constructible_v<Immobile>);

    // Nested prvalue still elided into the final object.
    auto outer = []() -> Immobile { return Immobile{make_immobile().value + 1}; };
    Immobile w = outer();
    assert(w.value == 124);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/guaranteed_copy_elision_cpp17", run>;

}  // namespace
