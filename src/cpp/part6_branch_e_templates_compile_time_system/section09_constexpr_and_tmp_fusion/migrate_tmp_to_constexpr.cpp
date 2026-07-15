// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E9 constexpr∩TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section09_constexpr_and_tmp_fusion
// Item     : migrate_tmp_to_constexpr
// Topic id : part6/e/section09/migrate_tmp_to_constexpr
//
// 要点: 值计算优先 constexpr; 类型计算仍用 TMP/traits/concepts。
// 参考: Boost.Hana 理念; C++14/17/20 constexpr 放宽

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>
#include <numeric>
#include <type_traits>

namespace {

// --- 旧: 类型级阶乘 ---
template <unsigned N>
struct Fact : std::integral_constant<unsigned, N * Fact<N - 1>::value> {};
template <>
struct Fact<0> : std::integral_constant<unsigned, 1> {};

// --- 新: constexpr 函数 ---
constexpr unsigned fact(unsigned n) {
    unsigned r = 1;
    for (unsigned i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

// --- 旧: 类型列表长度 ---
template <typename... Ts>
struct Len : std::integral_constant<std::size_t, sizeof...(Ts)> {};

// --- 新: constexpr 数组算法 ---
constexpr int sum_to(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i) {
        s += i;
    }
    return s;
}

constexpr auto make_iota_array() {
    std::array<int, 5> a{};
    for (int i = 0; i < 5; ++i) {
        a[static_cast<std::size_t>(i)] = i + 1;
    }
    return a;
}

// 仍适合 TMP: 类型选择
template <typename T>
using promote_t = std::conditional_t<std::is_integral_v<T> && sizeof(T) < sizeof(int), int, T>;

// 混合: constexpr 函数 + 类型参数
template <typename T>
constexpr T clamp_cx(T v, T lo, T hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// C++20: constexpr vector 风格用 array 演示
constexpr int max_in(const std::array<int, 4>& a) {
    int m = a[0];
    for (std::size_t i = 1; i < a.size(); ++i) {
        if (a[i] > m) {
            m = a[i];
        }
    }
    return m;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E9 migrate TMP to constexpr ===\n";

    static_assert(Fact<6>::value == fact(6));
    static_assert(fact(6) == 720);
    static_assert(sum_to(10) == 55);
    static_assert(Len<int, char, double>::value == 3);

    constexpr auto a = make_iota_array();
    static_assert(a[0] == 1 && a[4] == 5);
    static_assert(max_in(std::array<int, 4>{1, 9, 3, 4}) == 9);

    static_assert(std::is_same_v<promote_t<short>, int>);
    static_assert(std::is_same_v<promote_t<double>, double>);
    static_assert(clamp_cx(15, 0, 10) == 10);

    assert(fact(5) == 120);
    std::cout << "  value-level: constexpr loops; type-level: traits still\n";
    std::cout << "  result: shorter code, better errors, same compile-time power\n";
    std::cout << "migrate_tmp_to_constexpr: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section09/migrate_tmp_to_constexpr", run>;

}  // namespace
