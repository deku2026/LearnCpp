// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : initialization_phases
// Topic id : part6/b/section01/initialization_phases
//
// Covers: zero-init, constant-init, dynamic init of statics

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

int zeroed;         // static storage: zero-initialized before dynamic init
int constant = 10;  // constant initialization

struct Counter {
    int n;
    Counter() : n(1) {}
};
Counter dynamic_obj;  // dynamic initialization

void demo_basics() {
    assert(zeroed == 0);
    assert(constant == 10);
}

void demo_intermediate() {
    assert(dynamic_obj.n == 1);
    static int once = 0;
    ++once;
    assert(once >= 1);
}

void demo_expert() {
    // Constant initialization happens before dynamic initialization.
    constexpr int k = 5;
    static int arr[k] = {};
    assert(arr[0] == 0);
    assert(sizeof(arr) / sizeof(arr[0]) == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/initialization_phases", run>;

}  // namespace
