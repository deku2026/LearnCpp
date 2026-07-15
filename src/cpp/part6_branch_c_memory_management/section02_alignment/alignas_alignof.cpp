// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : alignas_alignof
// Topic id : part6/c/section02/alignas_alignof
//
// Covers: alignas / alignof basics

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>

namespace {

void demo_basics() {
    assert(alignof(char) == 1);
    assert(alignof(int) >= 1);
    struct alignas(16) A {
        char c;
    };
    assert(alignof(A) == 16);
}

void demo_intermediate() {
    alignas(32) int x = 0;
    assert(reinterpret_cast<std::uintptr_t>(&x) % 32u == 0);
    assert(x == 0);
}

void demo_expert() {
    struct B {
        alignas(8) char c;
        int i;
    };
    assert(alignof(B) >= 8);
    B b{};
    assert(b.i == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/alignas_alignof", run>;

}  // namespace
