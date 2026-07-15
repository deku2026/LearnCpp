// Topic     : 泛型 lambda（C++14 auto 参数）
// Doc       : 第2部分-阶段3 · 步骤 5.6
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 参数写 auto → operator() 为成员函数模板；各 auto 独立推导；
//       与函数模板同源；C++ Insights 可见 template<class T> operator()。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [generic_lambda_cpp14] ===\n";

    // -------------------------------------------------------------------------
    // §入门：一个 lambda，多种实参类型
    // -------------------------------------------------------------------------
    auto print_size_like = [](const auto& x) {
        if constexpr (requires { x.size(); }) {
            return static_cast<int>(x.size());
        } else {
            return static_cast<int>(x);
        }
    };
    // 更朴素的入门：
    auto add = [](auto a, auto b) { return a + b; };
    assert(add(1, 2) == 3);
    assert(add(1.5, 2.5) == 4.0);
    assert(add(std::string{"a"}, std::string{"b"}) == "ab");
    std::cout << "[intro] auto params make operator() a template\n";

    // -------------------------------------------------------------------------
    // §进阶：const auto&、独立推导
    // -------------------------------------------------------------------------
    auto max2 = [](const auto& a, const auto& b) { return a < b ? b : a; };
    assert(max2(3, 9) == 9);
    assert(max2(std::string{"z"}, std::string{"a"}) == "z");

    // 两个 auto 各自推导，不必同一类型（只要表达式合法）
    auto pair_sum = [](auto a, auto b) { return a + b; };
    assert(pair_sum(1, 2.5) == 3.5);

    std::vector<int> v{1, 2, 3};
    auto front_or = [](const auto& c, auto fallback) { return c.empty() ? fallback : c.front(); };
    assert(front_or(v, 0) == 1);
    (void)print_size_like;
    std::cout << "[advanced] const auto&; independent deduction per parameter\n";

    // -------------------------------------------------------------------------
    // §专家：与模板 lambda、SFINAE/requires
    // -------------------------------------------------------------------------
    // 泛型 lambda 拿不到「类型名本身」做 static_assert 时，C++20 模板 lambda 更直接。
    // 无捕获泛型 lambda 不能转函数指针（函数指针不能是模板）。
    auto id = [](auto x) { return x; };
    static_assert(std::is_same_v<decltype(id(1)), int>);
    // int (*fp)(int) = id; // ❌

    // STL 算法天然适配
    std::vector<std::string> words{"aa", "b", "ccc"};
    std::sort(words.begin(), words.end(), [](const auto& L, const auto& R) { return L.size() < R.size(); });
    assert(words.front() == "b");

    std::cout << "[expert] generic lambda ≈ templated call op; no decay to function pointer\n";
    std::cout << "=== generic_lambda_cpp14: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/generic_lambda_cpp14", run>;

}  // namespace
