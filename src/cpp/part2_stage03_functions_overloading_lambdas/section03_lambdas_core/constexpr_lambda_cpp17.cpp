// Topic     : constexpr lambda（C++17）
// Doc       : 第2部分-阶段3 · 步骤 5.8
// cppreference: https://en.cppreference.com/cpp/language/lambda
//               https://en.cppreference.com/cpp/language/constexpr
//
// 要点: 体符合常量表达式要求时 operator() 隐式 constexpr；可 static_assert / 数组界；
//       也可显式写 constexpr；C++20 起 consteval 组合。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constexpr_lambda_cpp17] ===\n";

    // -------------------------------------------------------------------------
    // §入门：编译期调用
    // -------------------------------------------------------------------------
    auto square = [](int n) constexpr { return n * n; };
    static_assert(square(5) == 25);
    std::array<int, square(3)> arr{};  // 界 = 9
    assert(arr.size() == 9);
    std::cout << "[intro] constexpr lambda usable in static_assert / array bound\n";

    // -------------------------------------------------------------------------
    // §进阶：隐式 constexpr
    // -------------------------------------------------------------------------
    // C++17：只要体是合法 constexpr，即使不写关键字也是 constexpr 调用运算符
    auto add = [](int a, int b) { return a + b; };
    static_assert(add(2, 3) == 5);

    constexpr auto mul = [](int a, int b) { return a * b; };
    static_assert(mul(4, 5) == 20);
    // 运行期也可调
    int x = 6;
    assert(mul(x, 7) == 42);
    std::cout << "[advanced] implicit constexpr call op when body qualifies\n";

    // -------------------------------------------------------------------------
    // §专家：捕获与常量性；consteval
    // -------------------------------------------------------------------------
    // 编译期求值时，捕获的状态也必须是常量表达式可用的
    constexpr int k = 3;
    // constexpr 变量在常量表达式中可直接读；此处显式 [k=k] 强调「捕获进闭包」
    constexpr auto add_k = [k = k](int n) { return n + k; };
    static_assert(add_k(4) == 7);

    // 捕获运行期值时只能运行期调用（不能 static_assert 这次调用）
    int runtime = 10;
    auto add_rt = [runtime](int n) { return n + runtime; };
    assert(add_rt(5) == 15);

    // C++20 consteval lambda：只允许编译期
    auto only_ce = [](int n) consteval { return n * 2; };
    static_assert(only_ce(3) == 6);
    // int y = 1; auto z = only_ce(y); // ❌ 非常量实参

    // 泛型 + constexpr
    auto abs_ce = [](auto v) constexpr { return v < 0 ? -v : v; };
    static_assert(abs_ce(-3) == 3);
    static_assert(abs_ce(4) == 4);

    std::cout << "[expert] captures must be usable at compile time for constexpr eval; consteval is stricter\n";
    std::cout << "=== constexpr_lambda_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/constexpr_lambda_cpp17", run>;

}  // namespace
