// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section02_if_consteval_cpp23
// Item     : if_consteval_versus_is_constant_evaluated
// Topic id : part2/stage07/section02/if_consteval_versus_is_constant_evaluated
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/if  (consteval if)
//   https://en.cppreference.com/w/cpp/types/is_constant_evaluated
//   P1938 (if consteval), P0595 (is_constant_evaluated)
//   ISO [stmt.if], [meta.const.eval]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 两个设施都在回答：当前是否「明显常量求值」？
// ---------------------------------------------------------------------------
// std::is_constant_evaluated() (C++20): *库函数*，配合 *普通 if*。
// if consteval (C++23): *语言构造*，true 分支是立即函数上下文。
//
// 共同用途：编译期走可 constexpr 的精确算法，运行期走硬件/快路径。

constexpr double power_ice(double base, int exp) {
    // 正确：普通 if + is_constant_evaluated
    if (std::is_constant_evaluated()) {
        double r = 1.0;
        const int n = exp < 0 ? -exp : exp;
        for (int i = 0; i < n; ++i) {
            r *= base;
        }
        return exp < 0 ? 1.0 / r : r;
    }
    return std::pow(base, static_cast<double>(exp));
}

constexpr double power_if_consteval(double base, int exp) {
    if consteval {
        double r = 1.0;
        const int n = exp < 0 ? -exp : exp;
        for (int i = 0; i < n; ++i) {
            r *= base;
        }
        return exp < 0 ? 1.0 / r : r;
    } else {
        return std::pow(base, static_cast<double>(exp));
    }
}

// ---------------------------------------------------------------------------
// §进阶 — 致命陷阱：if constexpr (is_constant_evaluated()) 恒为 true
// ---------------------------------------------------------------------------
// if constexpr 的条件本身在常量求值上下文中求值 →
// is_constant_evaluated() 在那里 *总是* true → else 被永久丢弃。

constexpr int buggy_always_compile_time_branch(int x) {
    // 演示「错误写法」的结构；我们不把运行期依赖放进 else，
    // 而是用标记返回值暴露分支选择。
    if (std::is_constant_evaluated()) {
        return x + 1000;  // 恒走这里（无论外层是否 CTCE）
    } else {
        return x + 1;  // 死代码
    }
}

constexpr int correct_ice_branch_marker(int x) {
    if (std::is_constant_evaluated()) {
        return x + 1000;
    }
    return x + 1;
}

constexpr int correct_if_consteval_marker(int x) {
    if consteval {
        return x + 1000;
    } else {
        return x + 1;
    }
}

// ---------------------------------------------------------------------------
// §专家 — 语义对照表（本文件重点）
// ---------------------------------------------------------------------------
// | 点                | is_constant_evaluated          | if consteval              |
// |-------------------|--------------------------------|---------------------------|
// | 引入              | C++20 库                       | C++23 语言                |
// | 头文件            | <type_traits> / <version>      | 无需                      |
// | 分支种类          | 普通 if                        | 特殊 if consteval         |
// | if constexpr 误用 | 恒 true（静默 bug）            | 语法上不会这样写          |
// | true 调 consteval | 否（普通上下文）               | 是（立即函数上下文）      |
// | 否定形式          | if (!is_constant_evaluated())  | if !consteval             |
//
// 本文件不调用 consteval（留给 if_consteval_calls_consteval_function.cpp），
// 专注「双路径选择 + 陷阱」。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section02/if_consteval_versus_is_constant_evaluated ===\n";

    // --- 入门：编译期路径 ---
    constexpr double a = power_ice(2.0, 10);
    constexpr double b = power_if_consteval(2.0, 10);
    static_assert(a == 1024.0);
    static_assert(b == 1024.0);
    std::cout << "[intro] compile-time power_ice(2,10)=" << a << " power_if_consteval(2,10)=" << b << '\n';

    // 运行期路径（实参非常量表达式变量）
    double base = 2.0;
    int exp = 8;
    const double r1 = power_ice(base, exp);
    const double r2 = power_if_consteval(base, exp);
    assert(r1 == 256.0);
    assert(r2 == 256.0);
    std::cout << "[intro] runtime power(2,8) ice=" << r1 << " if_consteval=" << r2 << '\n';

    // --- 进阶：分支标记 ---
    constexpr int ct_ice = correct_ice_branch_marker(5);
    constexpr int ct_ifc = correct_if_consteval_marker(5);
    static_assert(ct_ice == 1005);
    static_assert(ct_ifc == 1005);

    int x = 5;
    const int rt_ice = correct_ice_branch_marker(x);
    const int rt_ifc = correct_if_consteval_marker(x);
    assert(rt_ice == 6);
    assert(rt_ifc == 6);
    std::cout << "[advanced] markers CT: ice=" << ct_ice << " ifc=" << ct_ifc << " RT: ice=" << rt_ice
              << " ifc=" << rt_ifc << '\n';

    // buggy if constexpr 形式：无论 CT/RT 外层如何，函数内 if constexpr 条件恒 true
    constexpr int bug_ct = buggy_always_compile_time_branch(5);
    int y = 5;
    const int bug_rt = buggy_always_compile_time_branch(y);
    static_assert(bug_ct == 1005);
    assert(bug_rt == 1005);  // 运行期调用也得到 +1000 —— 静默错误模式
    std::cout << "[advanced] BUG if constexpr(is_constant_evaluated()): "
                 "CT="
              << bug_ct << " RT=" << bug_rt << " (both took the 'compile-time' arm)\n";

    // --- 专家：否定形式 if !consteval ---
    auto runtime_only_note = [](int v) constexpr -> const char* {
        if !consteval {
            return "runtime-path";
        } else {
            return "compile-time-path";
        }
        (void)v;
    };
    constexpr const char* p_ct = runtime_only_note(0);
    const char* p_rt = runtime_only_note(x);
    // 比较内容
    assert(p_ct[0] == 'c');
    assert(p_rt[0] == 'r');
    std::cout << "[expert] if !consteval: CT=\"" << p_ct << "\" RT=\"" << p_rt << "\"\n";

    std::cout << "[expert] prefer if consteval for new code: no header, no "
                 "if-constexpr footgun, immediate context for consteval calls\n";
    std::cout << "if_consteval_versus_is_constant_evaluated: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section02/if_consteval_versus_is_constant_evaluated", run>;

}  // namespace
