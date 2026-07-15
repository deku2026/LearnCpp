// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : lifetime_violation_deep
// Topic id : part6/g/section03/lifetime_violation_deep
//
// Covers: lifetime rules; do not use outside lifetime

#include "learn/topic_registry.hpp"

#include <memory>
#include <new>

namespace {

void demo_basics() {
    int x = 1;
    LEARN_CHECK(x == 1);
}  // x lifetime ends

void demo_intermediate() {
    alignas(int) unsigned char buf[sizeof(int)];
    int* p = new (buf) int(5);
    LEARN_CHECK(*p == 5);
    std::destroy_at(p);
}

void demo_expert() {
    // After explicit destructor, storage may be reused; do not use old glvalue casually.
    alignas(int) unsigned char buf[sizeof(int)];
    int* p = new (buf) int(1);
    std::destroy_at(p);
    p = new (buf) int(2);
    LEARN_CHECK(*p == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/lifetime_violation_deep", run>;

}  // namespace
