// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C4 over-aligned)
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : over_aligned_types
// Topic id : part6/c/section02/over_aligned_types
//
// 要点: 对齐 > max_align_t 为 over-aligned；C++17 new 走对齐版 operator new。
// 参考: operator new(size, align_val_t)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <new>

namespace {

struct alignas(64) LineBuf {
    char bytes[64];
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C4 over-aligned types ===\n";

    assert(alignof(LineBuf) > alignof(std::max_align_t) || alignof(LineBuf) >= 64);

    auto* p = new LineBuf{};
    auto addr = reinterpret_cast<std::uintptr_t>(p);
    assert(addr % alignof(LineBuf) == 0);
    p->bytes[0] = 'A';
    assert(p->bytes[0] == 'A');
    delete p;

    // 对齐版分配 API
    void* raw = ::operator new(128, std::align_val_t{64});
    auto raw_addr = reinterpret_cast<std::uintptr_t>(raw);
    assert(raw_addr % 64 == 0);
    ::operator delete(raw, std::align_val_t{64});

    std::cout << "  C++17+: new OverAligned uses aligned operator new\n";
    std::cout << "over_aligned_types: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/over_aligned_types", run>;

}  // namespace
