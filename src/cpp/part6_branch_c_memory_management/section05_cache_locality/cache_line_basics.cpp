// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : cache_line_basics
// Topic id : part6/c/section05/cache_line_basics
//
// Covers: cache line size concept; alignas(64) packing

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>

namespace {

struct alignas(64) Line {
    int value;
    char pad[60];
};

void demo_basics() {
    assert(alignof(Line) == 64);
    assert(sizeof(Line) == 64);
}

void demo_intermediate() {
    Line a{};
    Line b{};
    a.value = 1;
    b.value = 2;
    assert(a.value + b.value == 3);
    assert(reinterpret_cast<std::uintptr_t>(&a) % 64u == 0);
}

void demo_expert() {
    // Typical L1 cache line is 64 bytes on many CPUs (not guaranteed by C++).
    static_assert(sizeof(Line) == 64);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/cache_line_basics", run>;

}  // namespace
