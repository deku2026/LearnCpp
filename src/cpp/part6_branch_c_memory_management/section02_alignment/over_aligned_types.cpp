// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : over_aligned_types
// Topic id : part6/c/section02/over_aligned_types
//
// Covers: over-aligned types beyond __STDCPP_DEFAULT_NEW_ALIGNMENT__

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <memory>
#include <new>

namespace {

struct alignas(64) CacheLine {
    int data[4]{};
};

void demo_basics() {
    assert(alignof(CacheLine) == 64);
    CacheLine c{};
    assert(c.data[0] == 0);
}

void demo_intermediate() {
    auto p = std::make_unique<CacheLine>();
    assert(reinterpret_cast<std::uintptr_t>(p.get()) % 64u == 0);
}

void demo_expert() {
    void* raw = ::operator new(sizeof(CacheLine), std::align_val_t{64});
    auto* p = new (raw) CacheLine{};
    assert(reinterpret_cast<std::uintptr_t>(p) % 64u == 0);
    p->~CacheLine();
    ::operator delete(raw, std::align_val_t{64});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/over_aligned_types", run>;

}  // namespace
