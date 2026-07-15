// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section04_empty_base_and_no_unique_address
// Item     : no_unique_address_attribute_cpp20
// Topic id : part6/a/section04/no_unique_address_attribute_cpp20
//
// Covers: [[no_unique_address]] allows empty members to share storage (C++20)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>

namespace {

struct Empty {};

struct WithNua {
#if defined(_MSC_VER) && !defined(__clang__)
    [[msvc::no_unique_address]] Empty e;
#else
    [[no_unique_address]] Empty e;
#endif
    int x = 0;
};

struct WithoutNua {
    Empty e;
    int x = 0;
};

void demo_basics() {
    WithNua w;
    w.x = 5;
    assert(w.x == 5);
    static_assert(sizeof(Empty) >= 1);
}

void demo_intermediate() {
    // Optimization is permitted, not required; size should not exceed non-NUA.
    assert(sizeof(WithNua) <= sizeof(WithoutNua));
    // Common outcome: sizeof(WithNua) == sizeof(int).
    assert(sizeof(WithNua) == sizeof(int) || sizeof(WithNua) > sizeof(int));
}

void demo_expert() {
    WithNua a{};
    WithNua b{};
    // Distinct complete objects still have unique addresses.
    assert(&a != &b);
    // Two empty members of the same type still need distinct addresses if both present.
    struct TwoEmpty {
#if defined(_MSC_VER) && !defined(__clang__)
        [[msvc::no_unique_address]] Empty e1;
        [[msvc::no_unique_address]] Empty e2;
#else
        [[no_unique_address]] Empty e1;
        [[no_unique_address]] Empty e2;
#endif
        char c = 0;
    };
    TwoEmpty t{};
    assert(static_cast<const void*>(&t.e1) != static_cast<const void*>(&t.e2) || sizeof(TwoEmpty) >= 2);
    (void)t.c;
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section04/no_unique_address_attribute_cpp20", run>;

}  // namespace
