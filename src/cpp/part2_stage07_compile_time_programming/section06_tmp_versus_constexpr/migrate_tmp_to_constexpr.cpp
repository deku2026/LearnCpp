// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section06_tmp_versus_constexpr
// Item     : migrate_tmp_to_constexpr
// Topic id : part2/stage07/section06/migrate_tmp_to_constexpr
//
// Refs:
//   文档步骤 8：constexpr 与 TMP 的分工
//   https://en.cppreference.com/w/cpp/language/constexpr
//   https://en.cppreference.com/w/cpp/header/type_traits
//   《C++ Templates - The Complete Guide》TMP chapters

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 同一数值计算：TMP 递归 vs constexpr 函数
// ---------------------------------------------------------------------------

// 老：模板元编程阶乘
template <unsigned N>
struct FactorialTmp {
    static constexpr unsigned value = N * FactorialTmp<N - 1>::value;
};
template <>
struct FactorialTmp<0> {
    static constexpr unsigned value = 1;
};

// 新：constexpr 函数
constexpr unsigned factorial_cx(unsigned n) {
    unsigned r = 1;
    for (unsigned i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

// 斐波那契
template <unsigned N>
struct FibTmp {
    static constexpr unsigned value = FibTmp<N - 1>::value + FibTmp<N - 2>::value;
};
template <>
struct FibTmp<0> {
    static constexpr unsigned value = 0;
};
template <>
struct FibTmp<1> {
    static constexpr unsigned value = 1;
};

constexpr unsigned fib_cx(unsigned n) {
    if (n < 2) {
        return n;
    }
    unsigned a = 0;
    unsigned b = 1;
    for (unsigned i = 2; i <= n; ++i) {
        const unsigned next = a + b;
        a = b;
        b = next;
    }
    return b;
}

// ---------------------------------------------------------------------------
// §进阶 — 迁移策略：值走 constexpr，类型仍走 TMP / traits / concepts
// ---------------------------------------------------------------------------

// 类型计算：仍需模板（constexpr 函数不能「返回类型」作为一等结果）
template <class T>
struct AddConstPtr {
    using type = const T*;
};

// 值计算：GCD
constexpr unsigned gcd_cx(unsigned a, unsigned b) {
    while (b != 0) {
        const unsigned t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// 历史 TMP GCD
template <unsigned A, unsigned B>
struct GcdTmp {
    static constexpr unsigned value = GcdTmp<B, A % B>::value;
};
template <unsigned A>
struct GcdTmp<A, 0> {
    static constexpr unsigned value = A;
};

// 混合：类型层选算法，值层 constexpr 执行
template <class T>
constexpr T saturate_add(T a, T b) {
    static_assert(std::is_unsigned_v<T>);
    const T sum = static_cast<T>(a + b);
    // 无符号溢出回绕检测
    if (sum < a) {
        return static_cast<T>(-1);
    }
    return sum;
}

// ---------------------------------------------------------------------------
// §专家 — 何时保留 TMP；与 consteval / 变量模板 / 概念 的现代拼图
// ---------------------------------------------------------------------------
// 保留 TMP / 模板的场景：
//   - 产出类型（iterator_t、remove_cvref_t、函数特征分解）
//   - 重载集合裁剪（requires / 特化）
//   - 打包与反射前史（tuple 协议、index_sequence）
// 迁移到 constexpr 的场景：
//   - 任何「算一个值」：表生成、校验、数值、位运算、解析
// 现代写法：
//   - 变量模板：factorial_v<N> 作为语法糖，内部仍可调 constexpr
//   - consteval：强制编译期 API
//   - concepts：替代 enable_if 做约束，不替代数值 constexpr

template <unsigned N>
inline constexpr unsigned factorial_v = factorial_cx(N);

template <std::size_t... I>
constexpr auto make_iota_array(std::index_sequence<I...>) {
    // index_sequence 仍是模板设施；填值用 fold/constexpr
    return std::array<std::size_t, sizeof...(I)>{{I...}};
}

consteval unsigned must_be_ct(unsigned n) {
    return factorial_cx(n);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section06/migrate_tmp_to_constexpr ===\n";

    static_assert(FactorialTmp<5>::value == 120);
    static_assert(factorial_cx(5) == 120);
    static_assert(FactorialTmp<5>::value == factorial_cx(5));
    static_assert(FibTmp<10>::value == 55);
    static_assert(fib_cx(10) == 55);
    static_assert(GcdTmp<48, 18>::value == 6);
    static_assert(gcd_cx(48, 18) == 6);
    static_assert(factorial_v<6> == 720);
    static_assert(must_be_ct(4) == 24);

    using ACP = AddConstPtr<int>::type;
    static_assert(std::is_same_v<ACP, const int*>);

    static_assert(saturate_add(0xFFFFFFFFu, 1u) == 0xFFFFFFFFu);
    static_assert(saturate_add(3u, 4u) == 7u);

    constexpr auto iota = make_iota_array(std::make_index_sequence<5>{});
    static_assert(iota[0] == 0 && iota[4] == 4);

    std::cout << "[intro] FactorialTmp<5>=" << FactorialTmp<5>::value << " factorial_cx(5)=" << factorial_cx(5) << '\n';
    std::cout << "[intro] FibTmp<10>=" << FibTmp<10>::value << " fib_cx(10)=" << fib_cx(10) << '\n';
    std::cout << "[advanced] GcdTmp/gcd_cx(48,18)=" << gcd_cx(48, 18) << " AddConstPtr<int> is type-level TMP\n";
    std::cout << "[expert] factorial_v<6>=" << factorial_v<6> << " must_be_ct(4)=" << must_be_ct(4)
              << " iota size=" << iota.size() << '\n';

    std::cout << "[expert] RULE: values → constexpr/consteval; "
                 "types/overload sets → templates/traits/concepts\n";

    assert(factorial_cx(7) == 5040);
    std::cout << "migrate_tmp_to_constexpr: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section06/migrate_tmp_to_constexpr", run>;

}  // namespace
