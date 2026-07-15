// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E9 constexpr∩TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section09_constexpr_and_tmp_fusion
// Item     : if_consteval_in_metaprogramming_cpp23
// Topic id : part6/e/section09/if_consteval_in_metaprogramming_cpp23
//
// 要点: if consteval (C++23) 在编译期分支走 consteval 路径;
//       优于 is_constant_evaluated 的"假运行时"坑。
// 参考: P1938; cppreference if consteval

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

namespace {

// consteval 仅编译期
consteval int exact_pow2(int n) {
    int r = 1;
    for (int i = 0; i < n; ++i) {
        r *= 2;
    }
    return r;
}

// 编译期精确 / 运行时可用库函数
constexpr double my_sqrt(double x) {
    if consteval {
        // 朴素牛顿迭代 (仅编译期)
        if (x < 0) {
            return 0;
        }
        double g = x > 1 ? x : 1.0;
        for (int i = 0; i < 32; ++i) {
            g = 0.5 * (g + x / g);
        }
        return g;
    } else {
        return std::sqrt(x);
    }
}

// 与 is_constant_evaluated 对比
constexpr int pick_old(int x) {
    if (std::is_constant_evaluated()) {
        return x + 1;  // 编译期
    } else {
        return x + 2;  // 运行时
    }
}

constexpr int pick_new(int x) {
    if consteval {
        return x + 1;
    } else {
        return x + 2;
    }
}

// if consteval 内可调用 consteval 函数
constexpr int use_exact(int n) {
    if consteval {
        return exact_pow2(n);
    } else {
        int r = 1;
        for (int i = 0; i < n; ++i) {
            r *= 2;
        }
        return r;
    }
}

// 元编程: 编译期选类型策略值
template <typename T>
constexpr T identity_or_promote(T v) {
    if consteval {
        // 编译期保持原样
        return v;
    } else {
        return v;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E9 if consteval in metaprogramming (C++23) ===\n";

    constexpr double s2 = my_sqrt(2.0);
    static_assert(s2 > 1.414 && s2 < 1.415);

    double runtime = 9.0;
    assert(my_sqrt(runtime) == 3.0);

    constexpr int a = pick_old(10);
    constexpr int b = pick_new(10);
    static_assert(a == 11);
    static_assert(b == 11);
    assert(pick_old(10) == 12);
    assert(pick_new(10) == 12);

    constexpr int p = use_exact(5);
    static_assert(p == 32);
    assert(use_exact(3) == 8);

    assert(identity_or_promote(7) == 7);

    std::cout << "  if consteval: true branch is immediate context"
                 " (can call consteval)\n";
    std::cout << "  is_constant_evaluated: both branches must be valid always\n";
    std::cout << "if_consteval_in_metaprogramming_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section09/if_consteval_in_metaprogramming_cpp23", run>;

}  // namespace
