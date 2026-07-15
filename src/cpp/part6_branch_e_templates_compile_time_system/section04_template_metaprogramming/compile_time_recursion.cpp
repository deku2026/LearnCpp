// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E4 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : compile_time_recursion
// Topic id : part6/e/section04/compile_time_recursion
//
// 要点: 经典 TMP 用偏特化递归; C++14+ 可用 constexpr 函数更清晰。
// 参考: [temp.class.spec] constexpr recursion

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>

namespace {

// 阶乘 — 类型递归
template <unsigned N>
struct factorial : std::integral_constant<unsigned, N * factorial<N - 1>::value> {};

template <>
struct factorial<0> : std::integral_constant<unsigned, 1> {};

// 阶乘 — constexpr
constexpr unsigned fact_cx(unsigned n) {
    unsigned r = 1;
    for (unsigned i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

// 幂 — 模板递归
template <int Base, unsigned Exp>
struct power : std::integral_constant<int, Base * power<Base, Exp - 1>::value> {};

template <int Base>
struct power<Base, 0> : std::integral_constant<int, 1> {};

// 类型递归: 剥指针
template <typename T>
struct remove_all_pointers {
    using type = T;
};

template <typename T>
struct remove_all_pointers<T*> {
    using type = typename remove_all_pointers<T>::type;
};

// pack 递归求和
template <typename... Ts>
struct count_types : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <int... Ns>
struct sum;

template <>
struct sum<> : std::integral_constant<int, 0> {};

template <int H, int... T>
struct sum<H, T...> : std::integral_constant<int, H + sum<T...>::value> {};

// C++17 fold 替代递归
template <int... Ns>
constexpr int sum_fold = (... + Ns);

// 对数深度递归 (平衡) vs 线性深度
template <unsigned N>
struct fib : std::integral_constant<unsigned, fib<N - 1>::value + fib<N - 2>::value> {};

template <>
struct fib<0> : std::integral_constant<unsigned, 0> {};
template <>
struct fib<1> : std::integral_constant<unsigned, 1> {};

constexpr unsigned fib_cx(unsigned n) {
    if (n < 2) {
        return n;
    }
    unsigned a = 0, b = 1;
    for (unsigned i = 2; i <= n; ++i) {
        unsigned c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E4 compile-time recursion ===\n";

    static_assert(factorial<5>::value == 120);
    static_assert(fact_cx(5) == 120);
    static_assert(power<2, 10>::value == 1024);
    static_assert(std::is_same_v<remove_all_pointers<int****>::type, int>);
    static_assert(sum<1, 2, 3, 4>::value == 10);
    static_assert(sum_fold<1, 2, 3, 4> == 10);
    static_assert(fib<10>::value == 55);
    static_assert(fib_cx(10) == 55);

    assert(factorial<6>::value == 720);
    assert(fact_cx(6) == 720);

    std::cout << "  fact(5)=" << factorial<5>::value << " power(2,10)=" << power<2, 10>::value
              << " fib(10)=" << fib<10>::value << '\n';
    std::cout << "  prefer constexpr loops; keep TMP recursion for type-level work\n";
    std::cout << "compile_time_recursion: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/compile_time_recursion", run>;

}  // namespace
