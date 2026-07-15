// LearnCpp topic
// Doc      : 第6部分-支线G · -ftrapv
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : ftrapv
// Topic id : part6/g/section05/ftrapv
//
// 要点: -ftrapv 在有符号溢出时 trap (调试友好);
//       现代更常用 UBSan signed-integer-overflow。
// 参考: GCC -ftrapv

#include "learn/topic_registry.hpp"

#include <iostream>
#include <limits>

namespace {

[[maybe_unused]] int safe_inc(int x) {
    if (x == std::numeric_limits<int>::max()) return x;
    return x + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G11 -ftrapv ===\n";

    assert(safe_inc(41) == 42);
    assert(safe_inc(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());

    std::cout << "  -ftrapv inserts overflow checks (GCC; incomplete historically)\n";
    std::cout << "  prefer: -fsanitize=signed-integer-overflow / integer (UBSan)\n";
    std::cout << "  production: checked arithmetic helpers, not process traps\n";
    std::cout << "  signed overflow is UB in ISO C++; flags change the dialect\n";
    std::cout << "ftrapv: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/ftrapv", run>;

}  // namespace
