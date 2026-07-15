// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C4 aligned_alloc)
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : std_aligned_alloc
// Topic id : part6/c/section02/std_aligned_alloc
//
// 要点: 标准对齐分配 — C11 aligned_alloc 或 C++17 对齐 operator new。
//       MSVC STL 常不提供 std::aligned_alloc；本文件用可移植路径。
// 参考: https://en.cppreference.com/w/cpp/memory/c/aligned_alloc

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <new>

namespace {

void* portable_aligned_alloc(std::size_t alignment, std::size_t size) {
    return ::operator new(size, std::align_val_t{alignment});
}

void portable_aligned_free(void* p, std::size_t alignment) {
    ::operator delete(p, std::align_val_t{alignment});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C4 aligned allocation (portable) ===\n";

    constexpr std::size_t align = 64;
    constexpr std::size_t size = 128;
    void* mem = portable_aligned_alloc(align, size);
    assert(mem != nullptr);
    assert(reinterpret_cast<std::uintptr_t>(mem) % align == 0);

    auto* bytes = static_cast<unsigned char*>(mem);
    bytes[0] = 0xAB;
    assert(bytes[0] == 0xAB);
    portable_aligned_free(mem, align);

    // C11 规则备忘: alignment 为 2 的幂；size 为 alignment 的倍数；用 free 释放
    // MSVC CRT 历史: _aligned_malloc / _aligned_free（平台扩展）
    std::cout << "  portable: ::operator new(size, align_val_t)\n";
    std::cout << "  C11 aligned_alloc: power-of-two align; size multiple of align\n";
    std::cout << "std_aligned_alloc: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/std_aligned_alloc", run>;

}  // namespace
