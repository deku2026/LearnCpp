// Topic     : C++23 继承构造的 CTAD
// Doc       : 第2部分-阶段4 · 步骤 8.6
// cppreference: https://en.cppreference.com/cpp/language/class_template_argument_deduction
// 提案      : P2582
//
// 要点: using Base::Base 继承构造时，C++23 允许对派生类模板做 CTAD；
//       标准效果等价于从继承构造生成推导指引。工具链若尚未实现 P2582，
//       可用显式 deduction guide 达到同样教学语义（本文件采用可移植写法）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class T>
struct Base {
    T value;
    explicit Base(T v) : value(std::move(v)) {}
    const T& get() const { return value; }
};

template <class T>
struct Derived : Base<T> {
    using Base<T>::Base;  // 继承构造
    int tag = 1;
};

// C++23 P2582：编译器应能从继承构造隐式生成类似指引。
// 显式写出以保证当前 clang-cl / 过渡工具链上 CTAD 可演示：
template <class T>
Derived(T) -> Derived<T>;

template <class T>
struct Wrap {
    T v;
    Wrap(T x) : v(std::move(x)) {}
};

// 探测：若某实现已支持「仅 using Base::Base、无显式指引」的 CTAD，可对照文档验证。
// 这里以「继承构造 +（隐式或显式）推导指引」为验收语义。

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [inherited_constructor_ctad_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：显式模板实参（始终可用）
    // -------------------------------------------------------------------------
    Derived<int> d1{42};
    assert(d1.get() == 42);
    assert(d1.tag == 1);
    std::cout << "[intro] Derived<int> get=" << d1.get() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：透过继承构造做 CTAD（P2582 语义）
    // -------------------------------------------------------------------------
    Derived d2{7};  // → Derived<int>
    static_assert(std::is_same_v<decltype(d2), Derived<int>>);
    assert(d2.get() == 7);
    assert(d2.tag == 1);

    Derived d3{std::string{"hi"}};  // → Derived<std::string>
    static_assert(std::is_same_v<decltype(d3), Derived<std::string>>);
    assert(d3.get() == "hi");
    std::cout << "[advanced] CTAD Derived get=" << d3.get() << '\n';

    // -------------------------------------------------------------------------
    // §专家：对照普通 CTAD；P2582 解决什么
    // -------------------------------------------------------------------------
    Wrap w{3.14};  // Wrap<double>
    static_assert(std::is_same_v<decltype(w), Wrap<double>>);

    Base b{std::string{"base"}};
    static_assert(std::is_same_v<decltype(b), Base<std::string>>);

    // 没有 P2582 / 没有指引时：Derived x{1} 无法从 Base(T) 推导 Derived 的 T。
    // P2582 把「继承来的构造」纳入类模板推导，少写一层样板。
    std::cout << "[expert] inherited-ctor CTAD (P2582 or guide) ok; Wrap/Base CTAD ok\n";
    std::cout << "=== inherited_constructor_ctad_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section07/inherited_constructor_ctad_cpp23", run>;

}  // namespace
