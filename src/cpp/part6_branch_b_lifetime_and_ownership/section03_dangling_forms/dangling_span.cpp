// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_span
// Topic id : part6/b/section03/dangling_span
//
// Covers: span does not own; keep underlying storage alive

#include "learn/topic_registry.hpp"

#include <cassert>
#include <span>
#include <vector>

namespace {

void demo_basics() {
    int a[] = {1, 2, 3};
    std::span<int> s{a};
    assert(s.size() == 3);
    assert(s[0] == 1);
}

void demo_intermediate() {
    std::vector<int> v{10, 20};
    std::span<int> s{v};
    assert(s[1] == 20);
    // If v is destroyed or reallocated, s dangles — keep v alive.
    v.push_back(30);
    s = std::span<int>{v};  // refresh after reallocation risk
    assert(s.size() == 3);
}

void demo_expert() {
    // SAFE contrast: copy data instead of returning span to local array.
    std::vector<int> own{1, 2, 3, 4};
    std::span<const int> view = own;
    int sum = 0;
    for (int x : view) sum += x;
    assert(sum == 10);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_span", run>;

}  // namespace
