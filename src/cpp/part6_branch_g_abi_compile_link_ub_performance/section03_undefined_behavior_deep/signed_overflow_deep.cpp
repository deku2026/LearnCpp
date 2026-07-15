// LearnCpp topic
// Doc      : 第6部分-支线G · 有符号溢出
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : signed_overflow_deep
// Topic id : part6/g/section03/signed_overflow_deep
//
// 要点: 有符号整数溢出 = UB; 无符号是模 2^n 回绕 (已定义)。
// 不触发有符号溢出。

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <limits>

namespace {

[[maybe_unused]] bool mul_ok(std::int32_t a, std::int32_t b, std::int32_t& out) {
    if (a == 0 || b == 0) {
        out = 0;
        return true;
    }
    // 粗略检查
    if (a > 0 && b > 0 && a > std::numeric_limits<std::int32_t>::max() / b) {
        return false;
    }
    if (a > 0 && b < 0 && b < std::numeric_limits<std::int32_t>::min() / a) {
        return false;
    }
    if (a < 0 && b > 0 && a < std::numeric_limits<std::int32_t>::min() / b) {
        return false;
    }
    if (a < 0 && b < 0 && a < std::numeric_limits<std::int32_t>::max() / b) {
        return false;
    }
    out = a * b;
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G signed overflow (safe patterns) ===\n";

    [[maybe_unused]] std::int32_t r = 0;
    assert(mul_ok(1000, 1000, r));
    assert(r == 1'000'000);
    assert(!mul_ok(std::numeric_limits<std::int32_t>::max(), 2, r));

    // 无符号回绕: 已定义
    std::uint32_t u = std::numeric_limits<std::uint32_t>::max();
    u = u + 1u;
    assert(u == 0);

    // 有符号: 不写 INT_MAX+1
    [[maybe_unused]] int a = std::numeric_limits<int>::max();
    assert(a > 0);
    // int bad = a + 1; // UB

    std::cout << "  UBSan: -fsanitize=signed-integer-overflow\n";
    std::cout << "  -fwrapv makes signed wrap (non-standard dialect)\n";
    std::cout << "signed_overflow_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/signed_overflow_deep", run>;

}  // namespace
