// Topic     : auto 与初始化形式（含 {} 特例）
// Doc       : 第2部分-阶段2 · 步骤 1 + 6.1 预热
// cppreference: https://en.cppreference.com/cpp/language/auto
//               https://en.cppreference.com/cpp/language/list_initialization
//               https://en.cppreference.com/cpp/utility/initializer_list
//
// 要点: auto x = expr 按模板按值推导，剥顶层 const/引用；
//       auto x = {a,b} → initializer_list；C++17 起 auto x{v} 单元素为元素类型。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [auto_with_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：auto + 等号
    // -------------------------------------------------------------------------
    auto i = 42;    // int
    auto d = 3.14;  // double
    auto s = std::string{"hi"};
    auto b = true;  // bool
    static_assert(std::is_same_v<decltype(i), int>);
    static_assert(std::is_same_v<decltype(d), double>);
    static_assert(std::is_same_v<decltype(s), std::string>);
    assert(i == 42 && s == "hi" && b);
    std::cout << "[intro] auto from = expr OK\n";

    // -------------------------------------------------------------------------
    // 进阶：丢弃顶层 const/引用；保留底层 const
    // -------------------------------------------------------------------------
    const int ci = 7;
    int x = 1;
    int& rx = x;
    const int& cr = ci;

    auto a1 = ci;  // int（丢 const）
    auto a2 = rx;  // int（丢引用）
    auto a3 = cr;  // int（都丢）
    static_assert(std::is_same_v<decltype(a1), int>);
    static_assert(std::is_same_v<decltype(a2), int>);
    static_assert(std::is_same_v<decltype(a3), int>);

    const int* pc = &ci;
    auto a4 = pc;  // const int*：底层 const 保留
    static_assert(std::is_same_v<decltype(a4), const int*>);

    // 需要引用时显式写
    auto& r1 = x;         // int&
    const auto& r2 = ci;  // const int&
    auto&& fwd_l = x;     // int&（转发引用绑左值）
    auto&& fwd_r = 10;    // int&&
    static_assert(std::is_same_v<decltype(r1), int&>);
    static_assert(std::is_same_v<decltype(fwd_l), int&>);
    static_assert(std::is_same_v<decltype(fwd_r), int&&>);
    r1 = 99;
    assert(x == 99);
    std::cout << "[advanced] auto drops top-level const/ref; keep with &/const\n";

    // -------------------------------------------------------------------------
    // 专家：auto 与花括号的特殊规则（C++11/17）
    // -------------------------------------------------------------------------
    auto list = {1, 2, 3};  // std::initializer_list<int>
    static_assert(std::is_same_v<decltype(list), std::initializer_list<int>>);
    assert(list.size() == 3);

    auto single{1};  // C++17：直接列表初始化单元素 → int（不是 list）
    static_assert(std::is_same_v<decltype(single), int>);
    assert(single == 1);

    // auto multi{1, 2}; // ❌ C++17 起：多元素 auto{...} 非法

    auto eq_brace = {1};  // 仍然是 initializer_list（有 =）
    static_assert(std::is_same_v<decltype(eq_brace), std::initializer_list<int>>);

    // 实践建议：
    // - 想要元素列表容器：std::vector<int> v{1,2,3}; 或 auto v = std::vector{1,2,3};
    // - 避免 auto x = {1,2,3}; 除非真需要 initializer_list
    auto v = std::vector{1, 2, 3};  // CTAD
    assert(v.size() == 3 && v[0] == 1);

    // C++23 auto(x) decay-copy 见 section06；此处仅点到
    std::cout << "[expert] auto = {..} → initializer_list; auto{one} → element (C++17)\n";
    std::cout << "=== auto_with_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/auto_with_initialization", run>;

}  // namespace
