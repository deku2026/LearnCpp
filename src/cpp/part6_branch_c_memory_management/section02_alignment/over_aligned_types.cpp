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

    std::cout << "  alignof(LineBuf)=" << alignof(LineBuf) << " max_align_t=" << alignof(std::max_align_t) << '\n';
    assert(alignof(LineBuf) >= 64);

    auto* p = new LineBuf{};
    [[maybe_unused]] auto addr = reinterpret_cast<std::uintptr_t>(p);
    assert(addr % alignof(LineBuf) == 0);
    p->bytes[0] = 'A';
    assert(p->bytes[0] == 'A');
    delete p;

    void* raw = ::operator new(128, std::align_val_t{64});
    assert(reinterpret_cast<std::uintptr_t>(raw) % 64 == 0);
    ::operator delete(raw, std::align_val_t{64});

    // 专家: SIMD/缓存行/DMA 常需 over-alignment；
    // C++17 前 new 可能不保证 > max_align_t（依赖实现）
    std::cout << "  C++17+: new OverAligned uses aligned operator new\n";
    std::cout << "over_aligned_types: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/over_aligned_types", run>;

}  // namespace
