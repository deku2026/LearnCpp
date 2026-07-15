// Topic    : 引用折叠：模板 / auto / using / decltype 中的 && 规则
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 4.4 / 6
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : reference_collapsing
// Topic id : part2/stage02/section06/reference_collapsing
// Refs     : https://en.cppreference.com/cpp/language/reference
//            ISO [dcl.ref]
//            《Effective Modern C++》条款 24、28

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>
#include <utility>

namespace {

// 官方四条（using 别名 + 再叠一层引用）
using lref = int&;
using rref = int&&;

// 在模板参数里演示折叠结果
template <class T>
struct collapse_add_lref {
    using type = T&;
};
template <class T>
struct collapse_add_rref {
    using type = T&&;
};

template <class T>
void probe_forwarding(T&& /*x*/, const char* label) {
    // T&& 是转发引用时：左值 → T=U&，折叠成 U&；右值 → T=U，T&& 为 U&&
    using plain = std::remove_reference_t<T>;
    constexpr bool is_lvalue_param = std::is_lvalue_reference_v<T>;
    std::cout << "  " << label << ": T is " << (is_lvalue_param ? "U& (lvalue arg)" : "U (rvalue arg)")
              << ", T&& collapses to " << (std::is_lvalue_reference_v<T&&> ? "lvalue ref" : "rvalue ref") << ", plain~"
              << sizeof(plain) << "B\n";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [reference_collapsing] 入门：四条折叠表 ===\n";
    {
        // 手写 int& & 非法；在 using/typedef/auto/模板/decltype 中产生时折叠：
        // &  &  → &
        // &  && → &
        // && &  → &
        // && && → &&
        // 口诀：沾上左值引用就是左值引用；两个右值引用才是右值引用

        int n = 0;

        lref& r1 = n;   // int&  &  → int&
        lref&& r2 = n;  // int&  && → int&
        rref& r3 = n;   // int&& &  → int&
        rref&& r4 = 1;  // int&& && → int&&

        static_assert(std::is_same_v<decltype(r1), int&>);
        static_assert(std::is_same_v<decltype(r2), int&>);
        static_assert(std::is_same_v<decltype(r3), int&>);
        static_assert(std::is_same_v<decltype(r4), int&&>);

        // 类型层断言（不依赖对象）
        static_assert(std::is_same_v<collapse_add_lref<int&>::type, int&>);
        static_assert(std::is_same_v<collapse_add_lref<int&&>::type, int&>);
        static_assert(std::is_same_v<collapse_add_rref<int&>::type, int&>);
        static_assert(std::is_same_v<collapse_add_rref<int&&>::type, int&&>);

        assert(&r1 == &n && &r2 == &n && &r3 == &n);
        assert(r4 == 1);
        std::cout << "[intro] only && && stays rvalue ref\n";
    }

    std::cout << "=== 进阶：转发引用 T&& 依赖折叠 ===\n";
    {
        int a = 5;
        probe_forwarding(a, "lvalue a");
        probe_forwarding(10, "prvalue 10");
        probe_forwarding(std::move(a), "xvalue move(a)");

        // auto&& 同样是转发引用
        auto&& ar1 = a;   // int&
        auto&& ar2 = 10;  // int&&
        static_assert(std::is_same_v<decltype(ar1), int&>);
        static_assert(std::is_same_v<decltype(ar2), int&&>);
        std::cout << "[advanced] forwarding ref = deduced T&& + collapsing\n";
    }

    std::cout << "=== 专家：const T&& 不是转发引用；折叠与 remove_reference ===\n";
    {
        // const T&& 不是转发引用：T 仍推导，但参数只收右值且带 const
        // template<class T> void f(const T&&);  ← 不是 universal reference

        using R1 = std::add_lvalue_reference_t<int&&>;  // int&
        using R2 = std::add_rvalue_reference_t<int&>;   // int&
        using R3 = std::add_rvalue_reference_t<int&&>;  // int&&
        static_assert(std::is_same_v<R1, int&>);
        static_assert(std::is_same_v<R2, int&>);
        static_assert(std::is_same_v<R3, int&&>);

        // 这是 std::forward 能「保持值类别」的类型代数基础（见下一题）
        std::cout << "[expert] collapsing is the algebra behind std::forward\n";
    }

    std::cout << "[reference_collapsing] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/reference_collapsing", run>;

}  // namespace
