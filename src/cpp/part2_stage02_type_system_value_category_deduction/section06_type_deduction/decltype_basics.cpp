// Topic    : decltype：精确保留类型；对带括号的名字敏感
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 6.2
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : decltype_basics
// Topic id : part2/stage02/section06/decltype_basics
// Refs     : https://en.cppreference.com/cpp/language/decltype
//            ISO [dcl.type.decltype]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

struct A {
    double x;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [decltype_basics] 入门：标识符 → 声明类型 ===\n";
    {
        int x = 0;
        const int cx = 1;
        int& rx = x;
        const int& crx = cx;

        static_assert(std::is_same_v<decltype(x), int>);
        static_assert(std::is_same_v<decltype(cx), const int>);
        static_assert(std::is_same_v<decltype(rx), int&>);
        static_assert(std::is_same_v<decltype(crx), const int&>);

        // 未加括号的标识符：给出实体「声明时的类型」，不剥 const/引用
        std::cout << "[intro] decltype(name) is the declared type (exact)\n";
    }

    std::cout << "=== 进阶：带括号的名字 → 按值类别；成员访问官方例子 ===\n";
    {
        int x = 0;
        // 括号让标识符变成「表达式」→ lvalue → T&
        static_assert(std::is_same_v<decltype(x), int>);
        static_assert(std::is_same_v<decltype((x)), int&>);

        // 官方：成员访问 vs 括号成员访问
        const A* a = nullptr;
        (void)a;
        // decltype(a->x)   → double（成员声明类型）
        // decltype((a->x)) → const double&（(a->x) 是 lvalue；a 为 const → const double&）
        static_assert(std::is_same_v<decltype(std::declval<const A*>()->x), double>);
        // 用真实对象演示运行期绑定
        A obj{3.14};
        const A* pa = &obj;
        decltype(pa->x) y = 1.0;  // double
        decltype((pa->x)) z = y;  // const double&
        static_assert(std::is_same_v<decltype(y), double>);
        static_assert(std::is_same_v<decltype(z), const double&>);
        assert(z == 1.0);
        // z = 2.0;  // ❌ const

        std::cout << "[advanced] decltype((x)) is T& because (x) is an lvalue expression\n";
    }

    std::cout << "=== 专家：一般表达式按 lvalue→T& / xvalue→T&& / prvalue→T ===\n";
    {
        int i = 1;
        // 算术结果是 prvalue → int
        static_assert(std::is_same_v<decltype(i + 1), int>);
        // std::move(i) 是 xvalue → int&&
        static_assert(std::is_same_v<decltype(std::move(i)), int&&>);
        // 前置 ++ 是 lvalue → int&
        static_assert(std::is_same_v<decltype(++i), int&>);

        // 函数返回值形态决定
        auto by_value = []() -> int { return 0; };
        auto by_lref = [&]() -> int& { return i; };
        auto by_rref = [&]() -> int&& { return std::move(i); };
        static_assert(std::is_same_v<decltype(by_value()), int>);
        static_assert(std::is_same_v<decltype(by_lref()), int&>);
        static_assert(std::is_same_v<decltype(by_rref()), int&&>);

        // 文档步骤 0 手段 B：故意触发编译错误让编译器「说出」类型（Scott Meyers 条款 4）
        //   template<class T> struct TypeDisplayer; // 只声明不定义
        //   TypeDisplayer<decltype(expr)> _;  // 报错信息里带完整类型
        // 手段 A（本文件主力）: static_assert(is_same_v<...>)
        // 手段 C 慎用: typeid 会剥顶层 const/引用，不能用来判「有没有 &」
        using Guess = decltype((i));
        static_assert(std::is_same_v<Guess, int&>);
        // 若写成 TypeDisplayer<Guess> 未定义实例，clang/msvc 会打印 int& —— 自学时可用

        // 验收点一句话：括号把「名字」变成「表达式」，落入值类别规则
        std::cout << "[expert] unparenthesized id → declared type; "
                     "any other expr → value-category rule\n";
        (void)by_value;
        (void)by_lref;
        (void)by_rref;
    }

    std::cout << "[decltype_basics] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/decltype_basics", run>;

}  // namespace
