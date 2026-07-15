// LearnCpp topic
// Doc      : 第6部分-支线G · G11 -fno-strict-aliasing
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : fno_strict_aliasing
// Topic id : part6/g/section05/fno_strict_aliasing
//
// 要点: -fno-strict-aliasing 关闭基于类型的别名优化(TBAA);
//       遗留代码安全带, 不是新代码借口。新代码用 bit_cast/memcpy。
// 参考: GCC man -fstrict-aliasing

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace {

[[maybe_unused]] std::uint32_t as_u32(float f) {
    return std::bit_cast<std::uint32_t>(f);
}

[[maybe_unused]] std::uint32_t as_u32_memcpy(float f) {
    std::uint32_t u{};
    std::memcpy(&u, &f, sizeof u);
    return u;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G11 -fno-strict-aliasing (safety valve) ===\n";

    assert(as_u32(0.0f) == 0);
    assert(as_u32_memcpy(1.0f) == as_u32(1.0f));

    std::cout << "  flag disables TBAA optimizations (GCC/Clang)\n";
    std::cout << "  MSVC: different alias model; no direct equivalent switch\n";
    std::cout << "  prefer fixing punning with bit_cast/memcpy/char*\n";
    std::cout << "  Linux kernel historically built with -fno-strict-aliasing\n";
    std::cout << "  cost: may block load/store coalescing & reordering\n";
    std::cout << "fno_strict_aliasing: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/fno_strict_aliasing", run>;

}  // namespace
