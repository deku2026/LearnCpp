// Topic    : 继承构造 CTAD（C++23 P2582）—— 指引随 using Base::Base 继承
// Doc      : 第2部分-阶段6 · 步骤 8.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : inherited_ctor_ctad_cpp23
// Topic id : part2/stage06/section03/inherited_ctor_ctad_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
//            https://wg21.link/P2582R1
//            ISO [over.match.class.deduct]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <version>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 问题背景：using Base::Base 继承构造
// ---------------------------------------------------------------------------

template <typename T>
struct BaseBox {
    T value;
    BaseBox(T v) : value(std::move(v)) {}
};

template <typename T>
struct DerivedBox : BaseBox<T> {
    using BaseBox<T>::BaseBox;  // 继承构造
    int tag = 1;
};

// ---------------------------------------------------------------------------
// §进阶 — C++23 P2582：CTAD 应透过继承构造工作
// 本机/当前工具链对 P2582 支持不一，故始终提供显式 deduction guide 以保证可编译。
// 当编译器完整实现 P2582 后，该指引与语言规则冗余但不冲突。
// ---------------------------------------------------------------------------
template <typename T>
DerivedBox(T) -> DerivedBox<T>;

template <typename T>
struct ExplicitGuideDerived : BaseBox<T> {
    using BaseBox<T>::BaseBox;
};

// 始终可用的自定义指引（兼容 C++17+）
template <typename T>
ExplicitGuideDerived(T) -> ExplicitGuideDerived<T>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [inherited_ctor_ctad_cpp23] 入门：继承构造 ===\n";
    DerivedBox<int> explicit_type{42};
    assert(explicit_type.value == 42 && explicit_type.tag == 1);
    std::cout << "explicit DerivedBox<int> OK\n";

    std::cout << "=== 进阶：CTAD via explicit guide (P2582 fallback) ===\n";
    // 有显式指引后，从 42 / string 推出模板实参
    DerivedBox d{42};
    static_assert(std::is_same_v<decltype(d), DerivedBox<int>>);
    assert(d.value == 42 && d.tag == 1);
    DerivedBox ds{std::string{"x"}};
    static_assert(std::is_same_v<decltype(ds), DerivedBox<std::string>>);
    assert(ds.value == "x");
    std::cout << "DerivedBox CTAD via explicit guide OK (P2582 may be partial)\n";

    ExplicitGuideDerived eg{99};
    static_assert(std::is_same_v<decltype(eg), ExplicitGuideDerived<int>>);
    assert(eg.value == 99);

    ExplicitGuideDerived always{std::string{"ok"}};
    static_assert(std::is_same_v<decltype(always), ExplicitGuideDerived<std::string>>);
    assert(always.value == "ok");

    std::cout << "=== 专家：C++17/20 失败原因回顾 ===\n";
    // 继承构造的推导指引过去不被继承 → Derived d{42} 在 C++20 无法 CTAD
    // P2582R1 修复：继承构造时把基类指引一并纳入候选
    std::cout << "P2582: CTAD from inherited constructors; guide as fallback\n";
    std::cout << "=== inherited_ctor_ctad_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/inherited_ctor_ctad_cpp23", run>;

}  // namespace
