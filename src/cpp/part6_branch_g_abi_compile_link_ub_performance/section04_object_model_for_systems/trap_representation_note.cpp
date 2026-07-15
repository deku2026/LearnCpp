// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : trap_representation_note
// Topic id : part6/g/section04/trap_representation_note
//
// Covers: trap representations rare for common integer types; still initialize

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>

namespace {

void demo_basics() {
    std::uint32_t x = 0;
    assert(x == 0);
}

void demo_intermediate() {
    // Prefer value-initialization over indeterminate reads.
    int a[4]{};
    assert(a[3] == 0);
}

void demo_expert() {
    // Teaching: some old architectures had trap reps for ints; modern ILP32/LP64 less so.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/trap_representation_note", run>;

}  // namespace
