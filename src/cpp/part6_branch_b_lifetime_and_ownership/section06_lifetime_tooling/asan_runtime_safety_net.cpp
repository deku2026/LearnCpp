// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : asan_runtime_safety_net
// Topic id : part6/b/section06/asan_runtime_safety_net
//
// Covers: ASan as runtime safety net; safe code that would be caught if buggy

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

void demo_basics() {
    // ASan detects heap-buffer-overflow, use-after-free, etc. when enabled (-fsanitize=address).
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(v.at(0) == 1);
}

void demo_intermediate() {
    int* p = new int[4]{1, 2, 3, 4};
    LEARN_CHECK(p[3] == 4);
    delete[] p;
}

void demo_expert() {
    // We do not deliberately trigger ASan failures; use bounds-checked access.
    std::vector<int> v(2);
    v[0] = 1;
    v[1] = 2;
    LEARN_CHECK(v.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/asan_runtime_safety_net", run>;

}  // namespace
