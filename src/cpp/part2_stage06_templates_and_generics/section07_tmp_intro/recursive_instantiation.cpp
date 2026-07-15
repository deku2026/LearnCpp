// Topic    : 递归实例化 —— 编译期数值计算与终止特化
// Doc      : 第2部分-阶段6 · 步骤 13.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : recursive_instantiation
// Topic id : part2/stage06/section07/recursive_instantiation
// Refs     : https://en.cppreference.com/w/cpp/language/templates
//            ISO [temp.inst]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — Factorial 递归模板 + 全特化终止
// ---------------------------------------------------------------------------

template <unsigned N>
struct Factorial {
    static constexpr unsigned value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr unsigned value = 1;
};

// ---------------------------------------------------------------------------
// §进阶 — 斐波那契；幂；现代 constexpr 对照
// ---------------------------------------------------------------------------

template <unsigned N>
struct Fib {
    static constexpr unsigned value = Fib<N - 1>::value + Fib<N - 2>::value;
};
template <>
struct Fib<0> {
    static constexpr unsigned value = 0;
};
template <>
struct Fib<1> {
    static constexpr unsigned value = 1;
};

template <unsigned B, unsigned E>
struct Pow {
    static constexpr unsigned value = B * Pow<B, E - 1>::value;
};
template <unsigned B>
struct Pow<B, 0> {
    static constexpr unsigned value = 1;
};

constexpr unsigned factorial_cxpr(unsigned n) {
    return n == 0 ? 1 : n * factorial_cxpr(n - 1);
}

// ---------------------------------------------------------------------------
// §专家 — 无终止会爆编译；优先 constexpr 做数值
// ---------------------------------------------------------------------------

template <int N>
struct CountDown {
    static constexpr int value = CountDown<N - 1>::value;
};
template <>
struct CountDown<0> {
    static constexpr int value = 0;
};

// ⚠️ template<int N> struct Bad { static constexpr int v = Bad<N>::v; };
//    无递进终止 → 无限实例化

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [recursive_instantiation] 入门：Factorial ===\n";
    static_assert(Factorial<0>::value == 1);
    static_assert(Factorial<5>::value == 120);
    static_assert(Factorial<10>::value == 3628800);
    std::cout << "5!=" << Factorial<5>::value << '\n';

    std::cout << "=== 进阶：Fib / Pow / constexpr 对照 ===\n";
    static_assert(Fib<10>::value == 55);
    static_assert(Pow<2, 10>::value == 1024);
    static_assert(factorial_cxpr(5) == 120);
    // 数值计算：现代代码优先 constexpr 函数（阶段 7）
    // 递归模板仍是理解 type_traits / 类型计算的基础
    assert(Factorial<6>::value == factorial_cxpr(6));
    std::cout << "Fib(10)=" << Fib<10>::value << " 2^10=" << Pow<2, 10>::value << '\n';

    std::cout << "=== 专家：终止条件必须可达 ===\n";
    static_assert(CountDown<3>::value == 0);
    // 编译器通常有实例化深度限制（如 900/1024）；缺终止 → 灾难诊断
    std::cout << "always provide base-case specialization\n";
    std::cout << "=== recursive_instantiation: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/recursive_instantiation", run>;

}  // namespace
