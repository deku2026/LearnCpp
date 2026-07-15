// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C4 alignas/alignof)
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : alignas_alignof
// Topic id : part6/c/section02/alignas_alignof
//
// 要点: alignof 查询对齐；alignas 强制对齐（2 的幂）。
// 参考: [basic.align] [dcl.align]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <new>

namespace {

struct Normal {
    int a;
    double b;
};

struct alignas(64) CacheLine {
    int data[4];
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C4 alignas / alignof ===\n";

    static_assert(alignof(char) == 1);
    assert(alignof(int) >= 4);
    assert(alignof(double) >= 4);
    assert(alignof(Normal) == alignof(double));
    assert(alignof(CacheLine) == 64);
    assert(sizeof(CacheLine) % 64 == 0);

    alignas(32) int forced = 0;
    auto addr = reinterpret_cast<std::uintptr_t>(&forced);
    assert(addr % 32 == 0);
    (void)forced;

    std::cout << "  alignof(int)=" << alignof(int) << " alignof(CacheLine)=" << alignof(CacheLine)
              << " max_align_t=" << alignof(std::max_align_t) << '\n';

    std::cout << "alignas_alignof: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/alignas_alignof", run>;

}  // namespace
