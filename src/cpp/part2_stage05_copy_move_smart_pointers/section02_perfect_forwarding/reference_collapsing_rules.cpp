// Topic    : 引用折叠规则（reference collapsing）
// Doc      : 第2部分-阶段5 · 步骤 6.1；阶段 2 引用折叠
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : reference_collapsing_rules
// Topic id : part2/stage05/section02/reference_collapsing_rules
// Refs     : https://en.cppreference.com/w/cpp/language/reference
//            ISO [dcl.ref]；Effective Modern C++ Item 24

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

// 引用折叠四条（出现「引用的引用」时由语言折叠）：
//   T&  &   → T&
//   T&  &&  → T&
//   T&& &   → T&
//   T&& &&  → T&&
// 口诀：有左值引用参与 → 结果是左值引用；两个右值引用 → 右值引用

template <class T>
struct AddLRef {
    using type = T&;
};

template <class T>
struct AddRRef {
    using type = T&&;
};

// 模拟转发引用参数的「真实类型」
template <class T>
using FwdParam = T&&;  // 在模板里再经折叠

template <class T>
void show_collapse() {
    using L = typename AddLRef<T>::type;
    using R = typename AddRRef<T>::type;
    std::cout << "  collapse demo for a T\n";
    (void)sizeof(L);
    (void)sizeof(R);
}

// 通过函数模板参数看折叠后的实际形参类型
template <class T>
void inspect(T&& /*x*/) {
    // T&& 经过推导 + 折叠后的「形参类型」即 T（折叠结果）
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "  inspect: T is lvalue reference (folded to L-ref param)\n";
    } else {
        std::cout << "  inspect: T is non-reference (param is rvalue ref)\n";
    }
}

// typedef / using 触发折叠
template <class T>
using LRef = T&;

template <class T>
using RRef = T&&;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [reference_collapsing_rules] 入门：四条规则 ===\n";

    using T = int;
    static_assert(std::is_same_v<LRef<T>, int&>);
    static_assert(std::is_same_v<RRef<T>, int&&>);

    // 引用的引用经 using 组合：
    static_assert(std::is_same_v<LRef<int&>, int&>);    // & + &  → &
    static_assert(std::is_same_v<LRef<int&&>, int&>);   // && + & → &
    static_assert(std::is_same_v<RRef<int&>, int&>);    // & + && → &
    static_assert(std::is_same_v<RRef<int&&>, int&&>);  // && + && → &&

    std::cout << "static_assert: four collapsing rules OK\n";

    std::cout << "=== 进阶：转发引用如何利用折叠 ===\n";
    {
        int x = 0;
        inspect(x);             // T = int& ，T&& → int&
        inspect(1);             // T = int  ，T&& → int&&
        inspect(std::move(x));  // T = int  ，T&& → int&&

        const int cx = 2;
        inspect(cx);  // T = const int&
    }

    // decltype 与 std::forward 中的折叠
    {
        int x = 5;
        auto f = [](auto&& v) -> decltype(auto) {
            // 返回类型保持值类别（简化演示）
            return std::forward<decltype(v)>(v);
        };
        int& lr = f(x);
        int&& rr = f(10);
        assert(&lr == &x);
        assert(rr == 10);
        std::cout << "forward + decltype(auto) preserves category\n";
        (void)rr;
    }

    std::cout << "=== 专家：折叠只发生在类型层面 ===\n";
    // · 不能手写 int& && 变量（语法禁止「引用的引用」声明）
    // · 折叠出现在：模板类型推导、typedef/using、decltype 组合
    // · std::forward<T>(x) 内部用 static_cast<T&&>，再经折叠得到 T& 或 T&&
    // · remove_reference 后再 && 可强制右值引用（move 的做法）

    using A = int&;
    using B = A&&;  // 折叠为 int&
    static_assert(std::is_same_v<B, int&>);

    show_collapse<int>();
    show_collapse<int&>();

    std::cout << "=== reference_collapsing_rules: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/reference_collapsing_rules", run>;

}  // namespace
