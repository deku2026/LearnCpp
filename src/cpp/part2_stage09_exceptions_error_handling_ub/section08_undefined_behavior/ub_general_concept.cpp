// Topic    : 什么是 UB：编译器假设 UB 不发生并据此优化
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : ub_general_concept
// Topic id : part2/stage09/section08/ub_general_concept
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            ISO [intro.defs] (undefined behavior)
//            John Regehr UB series

#include "learn/topic_registry.hpp"

#include <cassert>
#include <climits>
#include <iostream>
#include <limits>

namespace {

// 经典“溢出检查被优化掉”的叙事（安全改写）：
// 若写成 if (x + 1 < x) —— 在有符号溢出是 UB 的前提下，编译器可认定条件恒假。
// 本函数用更宽类型做检查，行为良定义，作为正确对照。
int safe_inc(int x) {
    if (x == std::numeric_limits<int>::max()) {
        return -1;  // 明确拒绝，而不是依赖溢出
    }
    return x + 1;
}

// 展示：用无符号模运算做环绕（良定义），对比有符号溢出（UB，不演示）
unsigned wrap_add(unsigned a, unsigned b) {
    return a + b;  // 无符号溢出是 modulo 2^n，良定义
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [ub_general_concept] 主干：UB 是什么 ===\n";
    // 未定义行为：标准对某些操作不施加任何要求。
    // 一旦触发，整个程序行为不可预测（all bets are off）。
    // 可能“碰巧正常”、崩溃、错误结果、随优化级别变化。
    std::cout << "UB = no requirements from the abstract machine\n";
    std::cout << "compilers assume UB never happens, then optimize\n";

    std::cout << "=== 对抗：良定义的安全写法 ===\n";
    {
        assert(safe_inc(1) == 2);
        assert(safe_inc(std::numeric_limits<int>::max()) == -1);
        std::cout << "safe_inc(max) rejects instead of signed overflow\n";

        const unsigned u = wrap_add(UINT_MAX, 1u);
        assert(u == 0u);
        std::cout << "unsigned wrap UINT_MAX+1 -> " << u << " (defined)\n";
    }

    std::cout << "=== 专节：危险反例（只注释，不执行）===\n";
    // int f(int x) {
    //   if (x + 1 < x) return -1; // 编译器可删掉：假设有符号溢出不发生
    //   return x + 1;
    // }
    // godbolt: 对比 -O0 vs -O2 时，溢出检查分支常被优化掉。
    std::cout << "do not rely on signed overflow checks written as x+1<x\n";
    std::cout << "tooling: -fsanitize=undefined for many UB classes\n";

    std::cout << "[ub_general_concept] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/ub_general_concept", run>;

}  // namespace
