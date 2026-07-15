// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : core_guidelines_lifetime_profile
// Topic id : part6/b/section06/core_guidelines_lifetime_profile
//
// Covers: C++ Core Guidelines lifetime profile ideas in safe code

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <span>

namespace {

void demo_basics() {
    // I.11: never transfer ownership by raw pointer; use unique_ptr.
    auto p = std::make_unique<int>(1);
    assert(*p == 1);
}

void demo_intermediate() {
    // F.42: return T*; only if non-owning and lifetime clear.
    int x = 5;
    int* borrow = &x;
    assert(*borrow == 5);
}

void demo_expert() {
    int a[] = {1, 2, 3};
    std::span<int> s{a};
    assert(s.size() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/core_guidelines_lifetime_profile", run>;

}  // namespace
