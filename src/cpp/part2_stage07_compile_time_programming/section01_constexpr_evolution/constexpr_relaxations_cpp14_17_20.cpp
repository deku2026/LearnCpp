// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_relaxations_cpp14_17_20
// Topic id : part2/stage07/section01/constexpr_relaxations_cpp14_17_20
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/constexpr
//   https://en.cppreference.com/w/cpp/language/if (if constexpr, C++17)
//   P0784 (C++20 constexpr allocation / try-catch)
//   P1004 / P0980 (constexpr vector / string, covered deeper in section05)
//   ISO [dcl.constexpr] evolution notes

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — C++14：局部变量、循环、多语句 —— constexpr 开始「像普通函数」
// ---------------------------------------------------------------------------

constexpr int factorial_cpp14(int n) {
    int result = 1;  // C++14: 可变局部变量 OK
    for (int i = 2; i <= n; ++i) {
        result *= i;  // C++14: 循环 OK
    }
    return result;
}

constexpr int sum_to(int n) {
    int s = 0;
    int i = 1;
    while (i <= n) {  // while / do-while 均可
        s += i;
        ++i;
    }
    return s;
}

constexpr int classify_sign(int x) {
    // C++14: if / switch 作为语句
    if (x > 0) {
        return 1;
    }
    if (x < 0) {
        return -1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// §进阶 — C++17：if constexpr、constexpr lambda；C++14 起的成员修改
// ---------------------------------------------------------------------------

// C++14: 非静态 constexpr 成员函数不再隐含 const；可有 constexpr 非常量成员
struct Accumulator {
    int value = 0;
    constexpr Accumulator() = default;
    constexpr void add(int x) { value += x; }  // 非常量成员，C++14
    constexpr int get() const { return value; }
};

constexpr int accumulate_demo() {
    Accumulator acc;
    acc.add(10);
    acc.add(20);
    return acc.get();
}

// C++17: if constexpr —— 编译期分支丢弃，常用于泛型
template <class T>
constexpr auto double_or_length(const T& x) {
    if constexpr (std::is_integral_v<T>) {
        return x * 2;
    } else {
        return x.size();  // 假设 T 有 size()；丢弃分支不做实例化检查
    }
}

// C++17: lambda 可以是 constexpr（隐式或显式）
constexpr int lambda_square(int n) {
    auto sq = [](int x) constexpr { return x * x; };
    return sq(n);
}

// C++17: constexpr if + 折叠常一起做编译期算法（示意）
template <class... Ts>
constexpr int count_integrals() {
    return (0 + ... + (std::is_integral_v<Ts> ? 1 : 0));
}

// ---------------------------------------------------------------------------
// §专家 — C++20：虚函数、try/catch（不抛）、动态分配（瞬态）、union 等
// ---------------------------------------------------------------------------
// C++20 核心放宽（节选）：
//  - constexpr 虚函数 / 动态分配 (new/delete via construct_at 等)
//  - try/catch 可出现在 constexpr 函数中，但常量求值路径不能真正抛出
//  - std::vector / std::string 可 constexpr（瞬态分配，见 section05）
//  - 位域、部分 union 操作放宽
// 注意：本文件展示「能力边界」；容器细节留给 section05 专文。

struct Interface {
    constexpr virtual int id() const = 0;
    constexpr virtual ~Interface() = default;
};

struct ImplA : Interface {
    constexpr int id() const override { return 1; }
};

struct ImplB : Interface {
    constexpr int id() const override { return 2; }
};

constexpr int dispatch_virtual(bool pick_a) {
    ImplA a;
    ImplB b;
    const Interface* p = pick_a ? static_cast<const Interface*>(&a) : static_cast<const Interface*>(&b);
    return p->id();
}

constexpr int safe_div(int a, int b) {
    // try 可以出现；常量求值时不得抛出
    try {
        if (b == 0) {
            return 0;  // 不抛，保持核心常量表达式
        }
        return a / b;
    } catch (...) {
        return -1;  // 常量求值进不来这里
    }
}

constexpr int transient_vector_sum() {
    // C++20: constexpr vector 作为函数内工作区（求值结束前必须释放）
    std::vector<int> v{1, 2, 3, 4, 5};
    return std::accumulate(v.begin(), v.end(), 0);
}

constexpr int sort_array_sum() {
    std::array<int, 5> a{5, 1, 4, 2, 3};
    // C++20: 大量 <algorithm> 算法标了 constexpr
    std::sort(a.begin(), a.end());
    int s = 0;
    for (int x : a) {
        s += x;
    }
    return s;
}

// C++20: string 瞬态使用
constexpr std::size_t constexpr_string_len() {
    std::string s = "compile";
    s += "-time";
    return s.size();  // 12
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section01/constexpr_relaxations_cpp14_17_20 ===\n";

    // --- C++14 ---
    static_assert(factorial_cpp14(5) == 120);
    static_assert(sum_to(10) == 55);
    static_assert(classify_sign(-3) == -1);
    static_assert(accumulate_demo() == 30);
    std::cout << "[intro C++14] factorial_cpp14(5)=" << factorial_cpp14(5) << " sum_to(10)=" << sum_to(10)
              << " accumulate_demo=" << accumulate_demo() << '\n';

    // --- C++17 ---
    static_assert(double_or_length(21) == 42);
    static_assert(lambda_square(9) == 81);
    static_assert(count_integrals<int, double, long, void*>() == 2);
    std::string name = "Ada";
    assert(double_or_length(name) == 3);
    std::cout << "[advanced C++17] if constexpr double_or_length(21)=" << double_or_length(21)
              << " lambda_square(9)=" << lambda_square(9)
              << " count_integrals=" << count_integrals<int, double, long, void*>() << '\n';

    // --- C++20 ---
    static_assert(dispatch_virtual(true) == 1);
    static_assert(dispatch_virtual(false) == 2);
    static_assert(safe_div(20, 4) == 5);
    static_assert(safe_div(1, 0) == 0);
    static_assert(transient_vector_sum() == 15);
    static_assert(sort_array_sum() == 15);
    static_assert(constexpr_string_len() == 12);

    std::cout << "[expert C++20] virtual id=" << dispatch_virtual(true) << " vector_sum=" << transient_vector_sum()
              << " sorted_sum=" << sort_array_sum() << " string_len=" << constexpr_string_len() << '\n';

    std::cout << "[timeline] C++11 single-return → C++14 statements/loops → "
                 "C++17 if constexpr/lambda → C++20 virtual/alloc/containers\n";
    std::cout << "constexpr_relaxations_cpp14_17_20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_relaxations_cpp14_17_20", run>;

}  // namespace
