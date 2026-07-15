// Topic    : noexcept 运算符：编译期查询表达式是否不抛
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 2.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : noexcept_operator
// Topic id : part2/stage09/section02/noexcept_operator
// Refs     : https://en.cppreference.com/w/cpp/language/noexcept
//            ISO [expr.unary.noexcept]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void f() noexcept {}
void g() {}

struct Quiet {
    void foo() noexcept {}
};

struct Noisy {
    void foo() {}  // 可能抛
};

template <class T>
void wrapper(T t) noexcept(noexcept(t.foo())) {
    t.foo();
}

// 表达式层面的 noexcept 查询（不求值 operand）
template <class T>
constexpr bool can_nothrow_move_assign = noexcept(std::declval<T&>() = std::declval<T&&>());

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [noexcept_operator] 主干：noexcept(expr) 编译期 bool ===\n";
    {
        static_assert(noexcept(f()));
        static_assert(!noexcept(g()));
        // 运算符不求值 operand：仅作编译期查询
        f();
        g();
        std::cout << "noexcept(f())=" << noexcept(f()) << " noexcept(g())=" << noexcept(g()) << '\n';
    }

    std::cout << "=== 对抗：条件 noexcept 包装器 ===\n";
    {
        static_assert(noexcept(wrapper(Quiet{})));
        static_assert(!noexcept(wrapper(Noisy{})));
        wrapper(Quiet{});
        wrapper(Noisy{});
        std::cout << "wrapper(Quiet) nothrow=" << noexcept(wrapper(Quiet{}))
                  << " wrapper(Noisy) nothrow=" << noexcept(wrapper(Noisy{})) << '\n';
    }

    std::cout << "=== 对抗：标准类型的 noexcept 查询 ===\n";
    {
        static_assert(std::is_nothrow_move_constructible_v<std::string>);
        static_assert(std::is_nothrow_move_constructible_v<std::vector<int>>);
        // 字面量/算术通常 noexcept
        static_assert(noexcept(1 + 2));
        static_assert(can_nothrow_move_assign<std::string>);
        std::cout << "string nothrow_move_ctor=" << std::is_nothrow_move_constructible_v<std::string> << '\n';
    }

    std::cout << "=== 专节：与 type_traits 对照 ===\n";
    // noexcept 运算符查“这个表达式声明是否不抛”
    // is_nothrow_* 是针对类型特殊成员的 trait
    // 写泛型库时两者常一起用：传播 noexcept 契约
    std::cout << "use noexcept(expr) to propagate exception specs in templates\n";

    std::cout << "[noexcept_operator] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section02/noexcept_operator", run>;

}  // namespace
