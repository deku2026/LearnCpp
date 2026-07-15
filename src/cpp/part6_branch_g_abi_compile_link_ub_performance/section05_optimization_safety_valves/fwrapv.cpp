// LearnCpp topic
// Doc      : 第6部分-支线G · -fwrapv
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : fwrapv
// Topic id : part6/g/section05/fwrapv
//
// 要点: -fwrapv 将有符号溢出定义为二进制补码回绕 (方言);
//       不是可移植 ISO C++。应用显式 unsigned 或 checked 算术。
// 参考: GCC -fwrapv

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>

namespace {

[[maybe_unused]] std::uint32_t wrap_add(std::uint32_t a, std::uint32_t b) {
    return a + b;
}

[[maybe_unused]] bool checked_add(int a, int b, int& out) {
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) || (b < 0 && a < std::numeric_limits<int>::min() - b)) {
        return false;
    }
    out = a + b;
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G11 -fwrapv (signed wrap dialect) ===\n";

    assert(wrap_add(0xffffffffu, 1u) == 0u);
    [[maybe_unused]] int r = 0;
    assert(checked_add(1, 2, r) && r == 3);
    assert(!checked_add(std::numeric_limits<int>::max(), 1, r));

    std::cout << "  -fwrapv: treat signed overflow as two's complement wrap\n";
    std::cout << "  disables some loop opts that assume no overflow\n";
    std::cout << "  portable code: use unsigned or checked ops, not the flag\n";
    std::cout << "  related: -fno-strict-overflow (weaker dialect change)\n";
    std::cout << "fwrapv: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/fwrapv", run>;

}  // namespace
