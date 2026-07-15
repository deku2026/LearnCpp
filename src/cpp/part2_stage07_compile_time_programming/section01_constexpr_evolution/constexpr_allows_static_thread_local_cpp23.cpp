// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_allows_static_thread_local_cpp23
// Topic id : part2/stage07/section01/constexpr_allows_static_thread_local_cpp23
//
// Refs:
//   P2647R1 — Permitting static constexpr variables in constexpr functions
//   P2242R3 — Non-literal variables, labels, and goto in constexpr functions
//   https://en.cppreference.com/w/cpp/language/constexpr
//   Feature-test: __cpp_constexpr 202110L (P2242), 202211L (P2647)

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>

namespace {

// ---------------------------------------------------------------------------
// §入门 — C++23 允许 constexpr 函数内出现 static / thread_local 声明
// ---------------------------------------------------------------------------
// 旧规则：constexpr 函数体不得定义 static / thread_local 变量。
// C++23:
//   - P2242：允许声明存在（含非字面值局部变量、label、goto），
//     但 *常量求值路径* 仍不能执行到非法操作。
//   - P2647：进一步允许在常量表达式中使用 static constexpr 局部变量
//     （典型用途：编译期查找表）。

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L

constexpr char hex_digit(int n) {
    // 局部 static constexpr 查表：C++23 P2647 经典动机
    static constexpr char table[] = "0123456789abcdef";
    return table[n & 0xf];
}

constexpr int sum_lookup(int n) {
    // 更大一点的表：前缀和
    static constexpr int prefix[8] = {0, 1, 3, 6, 10, 15, 21, 28};
    if (n < 0 || n >= 8) {
        return -1;
    }
    return prefix[n];
}

#else

// 降级：命名空间作用域表，语义等价
constexpr char hex_digit(int n) {
    constexpr char table[] = "0123456789abcdef";
    return table[n & 0xf];
}

constexpr int sum_lookup(int n) {
    constexpr int prefix[8] = {0, 1, 3, 6, 10, 15, 21, 28};
    if (n < 0 || n >= 8) {
        return -1;
    }
    return prefix[n];
}

#endif

// ---------------------------------------------------------------------------
// §进阶 — static 可变 / thread_local：允许「存在」，常量求值勿踏入
// ---------------------------------------------------------------------------

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202110L

constexpr int count_calls_runtime_path(bool use_static_counter) {
    if (use_static_counter) {
        // 可变 static：常量求值若执行到这里通常不合法；
        // 运行期路径可以递增。
        static int counter = 0;
        return ++counter;
    }
    // 常量求值友好路径
    return 0;
}

// P2242 允许 constexpr 函数 *声明* 标签/goto；是否可在 CTCE 中 *执行*
// goto 仍受实现与 [expr.const] 约束。本函数给出可常量求值的等价控制流。
constexpr int demo_branch(int x) {
    if (x == 0) {
        return -1;
    }
    return x * 2;
}

// 运行期：展示函数体内可以出现 label（不参与 static_assert）
[[maybe_unused]] int runtime_goto_demo(int x) {
    if (x == 0) {
        goto zero_case;
    }
    return x * 2;
zero_case:
    return -1;
}

#else

constexpr int count_calls_runtime_path(bool use_static_counter) {
    if (use_static_counter) {
        return 1;  // 简化
    }
    return 0;
}

constexpr int demo_branch(int x) {
    if (x == 0) {
        return -1;
    }
    return x * 2;
}

int runtime_goto_demo(int x) {
    if (x == 0) {
        return -1;
    }
    return x * 2;
}

#endif

// ---------------------------------------------------------------------------
// §专家 — 常量求值语义 & 工程建议
// ---------------------------------------------------------------------------
// 1) static constexpr 局部表：每个函数一份，初始化是常量，适合纯函数查表。
// 2) 可变 static / thread_local：在常量求值中修改/读取往往违反 [expr.const]；
//    它们的价值在于「同一函数体服务运行期」，而不是编译期副作用。
// 3) 与 if consteval 组合：编译期走无 static 副作用路径，运行期走缓存路径。
// 4) thread_local 在 constexpr 函数中的声明同样遵循「存在合法、求值受限」。

constexpr int pure_path_only() {
    return hex_digit(10) == 'a' ? 1 : 0;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section01/constexpr_allows_static_thread_local_cpp23 ===\n";

    static_assert(hex_digit(0) == '0');
    static_assert(hex_digit(10) == 'a');
    static_assert(hex_digit(15) == 'f');
    static_assert(sum_lookup(4) == 10);
    static_assert(sum_lookup(7) == 28);
    static_assert(pure_path_only() == 1);
    static_assert(demo_branch(0) == -1);
    static_assert(demo_branch(5) == 10);
    assert(runtime_goto_demo(0) == -1);
    assert(runtime_goto_demo(5) == 10);

#if defined(__cpp_constexpr)
    std::cout << "[intro] __cpp_constexpr=" << __cpp_constexpr << '\n';
#endif
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    std::cout << "[intro] P2647 static constexpr local table enabled\n";
#else
    std::cout << "[intro] using namespace-scope table fallback\n";
#endif

    std::cout << "[intro] hex_digit(10)='" << hex_digit(10) << "' sum_lookup(4)=" << sum_lookup(4) << '\n';

    // 运行期：可变 static 计数
    const int c1 = count_calls_runtime_path(true);
    const int c2 = count_calls_runtime_path(true);
    assert(c1 >= 1);
    assert(c2 == c1 + 1 || c2 == 1);  // 支持 static 时递增；降级实现固定 1
    assert(count_calls_runtime_path(false) == 0);

    std::cout << "[advanced] runtime static counter path: c1=" << c1 << " c2=" << c2
              << " pure path=" << count_calls_runtime_path(false) << '\n';

    // 编译期表 vs 运行期表：同一 hex_digit
    std::array<char, 16> digits{};
    for (int i = 0; i < 16; ++i) {
        digits[static_cast<std::size_t>(i)] = hex_digit(i);
    }
    assert(digits[15] == 'f');
    std::cout << "[expert] filled runtime array via constexpr hex_digit; "
                 "static local table does not escape as mutable state in CTCE\n";

    std::cout << "constexpr_allows_static_thread_local_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section01/constexpr_allows_static_thread_local_cpp23", run>;

}  // namespace
