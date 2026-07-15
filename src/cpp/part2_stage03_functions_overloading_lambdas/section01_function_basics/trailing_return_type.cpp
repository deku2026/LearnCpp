// Topic     : 尾置返回类型 auto f() -> T
// Doc       : 第2部分-阶段3 · 步骤 4.1
// cppreference: https://en.cppreference.com/cpp/language/function
//               https://en.cppreference.com/cpp/language/decltype
//
// 要点: 返回类型写在参数列表之后；参数名已在作用域，可供 decltype；
//       复杂返回类型时函数名更靠前、更易读。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 与 int add(int,int) 等价的教学写法
auto add(int a, int b) -> int {
    return a + b;
}

// 用途一：返回类型依赖参数（参数声明之后才能 decltype）
template <class L, class R>
auto add_gen(const L& lhs, const R& rhs) -> decltype(lhs + rhs) {
    return lhs + rhs;
}

// 用途二：复杂返回类型时对齐可读性
// 前置：void (*fp_old())(const std::string&);  函数名淹没
auto make_printer() -> void (*)(const std::string&) {
    return [](const std::string& s) { std::cout << "[print] " << s << '\n'; };
}

// 类内嵌套类型也适合尾置
struct Box {
    using value_type = int;
    value_type v = 0;
};

auto get_value(const Box& b) -> Box::value_type {
    return b.v;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [trailing_return_type] ===\n";

    // -------------------------------------------------------------------------
    // §入门：基本语法
    // -------------------------------------------------------------------------
    assert(add(2, 3) == 5);
    static_assert(std::is_same_v<decltype(add(1, 2)), int>);
    std::cout << "[intro] auto add(int,int) -> int works like classic return type\n";

    // -------------------------------------------------------------------------
    // §进阶：依赖参数的返回类型
    // -------------------------------------------------------------------------
    auto r1 = add_gen(1, 2);    // int
    auto r2 = add_gen(1.5, 2);  // double（通常）
    auto r3 = add_gen(std::string{"a"}, std::string{"b"});
    assert(r1 == 3);
    assert(r2 > 3.4 && r2 < 3.6);
    assert(r3 == "ab");
    static_assert(std::is_same_v<decltype(add_gen(1, 2)), int>);
    std::cout << "[advanced] trailing decltype(lhs+rhs) deduces from parameter expressions\n";

    // -------------------------------------------------------------------------
    // §专家：可读性 + 与 auto/decltype(auto) 分工
    // -------------------------------------------------------------------------
    auto fp = make_printer();
    fp("trailing-return");
    Box b{42};
    assert(get_value(b) == 42);

    // 怎么选：
    // - 返回类型简单且固定 → 经典前置 int f() 或尾置均可；
    // - 依赖参数 / 嵌套类型很长 → 尾置 -> decltype(...) 或别名；
    // - C++14 起短函数可用 auto 推导（见下一 topic）；
    // - 完美转发返回值保留引用 → decltype(auto)，不是单纯尾置。
    static_assert(std::is_same_v<decltype(get_value(b)), Box::value_type>);

    std::cout << "[expert] use trailing when return depends on params or type is noisy\n";
    std::cout << "=== trailing_return_type: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/trailing_return_type", run>;

}  // namespace
