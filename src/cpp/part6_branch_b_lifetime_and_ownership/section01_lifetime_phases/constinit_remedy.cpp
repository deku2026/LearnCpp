// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : constinit_remedy
// Topic id : part6/b/section01/constinit_remedy
//
// Covers: constinit forces constant initialization (C++20)

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

constinit int g_flag = 0;
constexpr int k = 42;
constinit const int* g_ptr = &k;

void demo_basics() {
    assert(g_flag == 0);
    g_flag = 1;
    assert(g_flag == 1);
}

void demo_intermediate() {
    assert(g_ptr != nullptr);
    assert(*g_ptr == 42);
}

void demo_expert() {
    // constinit does not mean const; it means init is constant-initialization.
    constinit static int local = 5;
    assert(local == 5);
    local = 6;
    assert(local == 6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/constinit_remedy", run>;

}  // namespace
