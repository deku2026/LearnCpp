// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : fno_strict_aliasing
// Topic id : part6/g/section05/fno_strict_aliasing
//
// Covers: compiler flag -fno-strict-aliasing as emergency valve

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>

namespace {

void demo_basics() {
    // Prefer defined punning over disabling alias analysis globally.
    int x = 5;
    int y = 0;
    std::memcpy(&y, &x, sizeof(int));
    assert(y == 5);
}

void demo_intermediate() {
    assert(true);
}

void demo_expert() {
    // -fno-strict-aliasing may paper over bugs at a performance cost; fix code first.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/fno_strict_aliasing", run>;

}  // namespace
