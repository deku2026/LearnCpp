// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C4 aligned_alloc)
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : std_aligned_alloc
// Topic id : part6/c/section02/std_aligned_alloc
//
// 要点: 标准对齐分配——aligned_alloc(C11/C++17) 在 POSIX 常见；
//       MSVC STL 往往不提供 std::aligned_alloc，可移植路径用对齐版 operator new。
// 参考: https://en.cppreference.com/w/cpp/memory/c/aligned_alloc

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <new>

namespace {

void* portable_aligned_alloc(std::size_t alignment, std::size_t size) {
    // 可移植首选：C++17 对齐版 operator new（MSVC/clang-cl/libstdc++ 均可用）
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
    constexpr std::size_t size = 128;  // aligned_alloc 要求 size 为 alignment 倍数
    void* mem = portable_aligned_alloc(align, size);
    assert(mem != nullptr);
    auto addr = reinterpret_cast<std::uintptr_t>(mem);
    assert(addr % align == 0);

    auto* bytes = static_cast<unsigned char*>(mem);
    bytes[0] = 0xAB;
    assert(bytes[0] == 0xAB);
    portable_aligned_free(mem, align);

#if defined(__cpp_lib_constexpr_algorithms)  // just a feature-test presence probe
#endif
    // 文档对照：若平台提供 std::aligned_alloc(alignment, size)：
    //   alignment 须 2 的幂；size 须为 alignment 整数倍；用 std::free 释放。
    // MSVC CRT 传统上用 _aligned_malloc/_aligned_free（平台扩展，本支线不用）。

    std::cout << "  portable: ::operator new(size, align_val_t)\n";
    std::cout << "  C11 aligned_alloc: power-of-two align; size multiple of align\n";
    std::cout << "std_aligned_alloc: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/std_aligned_alloc", run>;

}  // namespace
