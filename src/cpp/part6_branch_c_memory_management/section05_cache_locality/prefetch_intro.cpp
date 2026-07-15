// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : prefetch_intro
// Topic id : part6/c/section05/prefetch_intro
//
// Covers: software prefetch is optional; locality via access order matters more

#include "learn/topic_registry.hpp"

#include <cassert>
#include <numeric>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v(128);
    std::iota(v.begin(), v.end(), 0);
    int sum = 0;
    for (int x : v) sum += x;
    assert(sum == 127 * 128 / 2);
}

void demo_intermediate() {
    // Sequential access is cache-friendly; random jumps are not.
    std::vector<int> v(64, 1);
    int sum = 0;
    for (std::size_t i = 0; i < v.size(); ++i) sum += v[i];
    assert(sum == 64);
}

void demo_expert() {
#if defined(__GNUC__) || defined(__clang__)
    int x = 1;
    __builtin_prefetch(&x, 0, 3);
    assert(x == 1);
#else
    int x = 1;
    assert(x == 1);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/prefetch_intro", run>;

}  // namespace
