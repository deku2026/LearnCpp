// Topic    : auto 按模板实参推导：丢顶层 const / 引用
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 6.1
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : auto_deduces_decays
// Topic id : part2/stage02/section06/auto_deduces_decays
// Refs     : https://en.cppreference.com/cpp/language/auto
//            《Effective Modern C++》条款 1–2
//            ISO [dcl.spec.auto]

#include "learn/topic_registry.hpp"

#include <initializer_list>
#include <iostream>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [auto_deduces_decays] 入门：按值 auto 丢顶层 const / 引用 ===\n";
    {
        int a = 0;
        const int ca = 0;
        int& ra = a;
        const int& cra = ca;

        auto x1 = a;    // int
        auto x2 = ca;   // int  ← 丢掉顶层 const
        auto x3 = ra;   // int  ← 丢掉引用
        auto x4 = cra;  // int  ← const 与引用都丢

        static_assert(std::is_same_v<decltype(x1), int>);
        static_assert(std::is_same_v<decltype(x2), int>);
        static_assert(std::is_same_v<decltype(x3), int>);
        static_assert(std::is_same_v<decltype(x4), int>);

        // 要保留引用 / const，显式加修饰
        auto& r1 = a;         // int&
        const auto& r2 = ca;  // const int&
        auto&& r3 = a;        // int&（转发引用绑左值）
        auto&& r4 = 10;       // int&&（转发引用绑右值）
        auto* p = &a;         // int*

        static_assert(std::is_same_v<decltype(r1), int&>);
        static_assert(std::is_same_v<decltype(r2), const int&>);
        static_assert(std::is_same_v<decltype(r3), int&>);
        static_assert(std::is_same_v<decltype(r4), int&&>);
        static_assert(std::is_same_v<decltype(p), int*>);

        assert(x1 == 0 && x2 == 0 && x3 == 0 && x4 == 0);
        std::cout << "[intro] auto strips top-level const/ref; keep with auto&/const auto&/auto&&\n";
    }

    std::cout << "=== 进阶：底层 const 保留；auto 与 {} 的 initializer_list 特例 ===\n";
    {
        // 底层 const 在指针层面：指向 const 的指针，const 是「所指对象」的属性
        const int n = 5;
        const int* cp = &n;
        auto q = cp;  // const int* —— 底层 const 保留，顶层无（指针本身可改指向）
        static_assert(std::is_same_v<decltype(q), const int*>);
        // *q = 1;  // ❌
        q = nullptr;  // OK：顶层无 const

        // auto x = {1,2,3} → initializer_list（特例，几乎从来不是你想要的）
        auto il = {1, 2, 3};
        static_assert(std::is_same_v<decltype(il), std::initializer_list<int>>);
        assert(il.size() == 3);

        // C++17 起：auto b{1}; 单元素直接列表 → 元素类型
        auto b{1};
        static_assert(std::is_same_v<decltype(b), int>);
        // auto c{1, 2};  // ❌ C++17+ 多元素 auto x{...} 非法

        auto d = 1;
        static_assert(std::is_same_v<decltype(d), int>);
        std::cout << "[advanced] low-level const kept; auto={...} is initializer_list\n";
    }

    std::cout << "=== 专家：为何按值丢信息；与模板 T 同源；推导全家福 ===\n";
    {
        // auto x = expr 规则 ≈ template<class T> void f(T param); f(expr);
        // 目标是「新的独立对象」，源的顶层 const/引用对副本无意义 → 剥掉
        const int& src = 42;
        auto copy = src;
        static_assert(std::is_same_v<decltype(copy), int>);
        copy = 7;  // OK：副本可改
        assert(src == 42 && copy == 7);

        // 文档扩展练习 2：对同一 const int& 源的推导全家福（验收用 static_assert）
        auto a0 = src;               // int
        auto& a1 = src;              // const int&（底层 const 保留）
        const auto& a2 = src;        // const int&
        auto&& a3 = src;             // const int&（转发引用绑左值）
        using D0 = decltype(src);    // const int&
        using D1 = decltype((src));  // const int&（(src) 是 lvalue 表达式）
        decltype(auto) a4 = src;     // const int&

        static_assert(std::is_same_v<decltype(a0), int>);
        static_assert(std::is_same_v<decltype(a1), const int&>);
        static_assert(std::is_same_v<decltype(a2), const int&>);
        static_assert(std::is_same_v<decltype(a3), const int&>);
        static_assert(std::is_same_v<D0, const int&>);
        static_assert(std::is_same_v<D1, const int&>);
        static_assert(std::is_same_v<decltype(a4), const int&>);
        assert(&a1 == &src && &a4 == &src);

        std::cout << "[expert] auto is template-arg deduction by value; "
                     "use Insights/static_assert, not typeid\n";
    }

    std::cout << "[auto_deduces_decays] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/auto_deduces_decays", run>;

}  // namespace
