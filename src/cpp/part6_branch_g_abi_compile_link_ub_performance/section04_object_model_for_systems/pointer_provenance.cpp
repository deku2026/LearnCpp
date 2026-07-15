// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : pointer_provenance
// Topic id : part6/g/section04/pointer_provenance
//
// Covers: pointer provenance: equality vs reachability intuition

#include "learn/topic_registry.hpp"

#include <cstdint>

namespace {

void demo_basics() {
    int a = 1;
    int b = 2;
    int* pa = &a;
    int* pb = &b;
    LEARN_CHECK(pa != pb);
    LEARN_CHECK(*pa == 1 && *pb == 2);
}

void demo_intermediate() {
    int arr[2] = {1, 2};
    int* p0 = &arr[0];
    int* p1 = p0 + 1;
    LEARN_CHECK(*p1 == 2);
    LEARN_CHECK(p1 - p0 == 1);
}

void demo_expert() {
    // Teaching: comparing / deriving pointers across unrelated objects has strict rules.
    // Stay within array bounds / object lifetime.
    int x = 0;
    auto addr = reinterpret_cast<std::uintptr_t>(&x);
    LEARN_CHECK(addr != 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/pointer_provenance", run>;

}  // namespace
