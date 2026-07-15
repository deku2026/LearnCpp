// Topic    : <type_traits> 入门：判断 _v 与变换 _t
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 7.2
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : type_traits_intro
// Topic id : part2/stage02/section07/type_traits_intro
// Refs     : https://en.cppreference.com/cpp/header/type_traits
//            https://en.cppreference.com/cpp/types/is_same
//            https://en.cppreference.com/cpp/types/decay

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 教学：用 traits 约束（预告 concepts；此处用 static_assert）
template <class T>
constexpr T twice(T x) {
    static_assert(std::is_arithmetic_v<T>, "twice expects arithmetic T");
    return static_cast<T>(x + x);
}

// 手写一层「去引用再加 const」管线
template <class T>
using as_const_value = std::add_const_t<std::remove_reference_t<T>>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [type_traits_intro] 入门：类型判断 _v ===\n";
    {
        static_assert(std::is_integral_v<int>);
        static_assert(std::is_pointer_v<int*>);
        static_assert(!std::is_same_v<int, long>);
        static_assert(std::is_const_v<const int>);
        static_assert(!std::is_const_v<int>);
        static_assert(std::is_reference_v<int&>);
        static_assert(std::is_rvalue_reference_v<int&&>);
        static_assert(std::is_lvalue_reference_v<int&>);
        static_assert(std::is_arithmetic_v<double>);
        static_assert(std::is_class_v<std::string>);

        // 老式 ::value 与现代 _v 等价
        static_assert(std::is_integral<int>::value == std::is_integral_v<int>);

        assert(twice(21) == 42);
        assert(twice(1.5) == 3.0);
        std::cout << "[intro] *_v traits answer compile-time yes/no\n";
    }

    std::cout << "=== 进阶：类型变换 _t（去引用/const、decay、加指针）===\n";
    {
        static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
        static_assert(std::is_same_v<std::remove_reference_t<int&&>, int>);
        static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
        static_assert(std::is_same_v<std::remove_cv_t<const volatile int>, int>);
        static_assert(std::is_same_v<std::add_pointer_t<int>, int*>);
        static_assert(std::is_same_v<std::add_lvalue_reference_t<int>, int&>);
        static_assert(std::is_same_v<std::add_rvalue_reference_t<int>, int&&>);

        // decay：去引用 + 顶层 cv + 数组/函数退化 —— auto(x) 的库侧语义
        static_assert(std::is_same_v<std::decay_t<const int&>, int>);
        static_assert(std::is_same_v<std::decay_t<int[3]>, int*>);
        static_assert(std::is_same_v<std::decay_t<int(int)>, int (*)(int)>);

        using A = as_const_value<int&>;
        static_assert(std::is_same_v<A, const int>);

        // 老式 ::type
        static_assert(std::is_same_v<typename std::remove_reference<int&>::type, int>);
        std::cout << "[advanced] *_t transforms types; decay_t matches auto(x)\n";
    }

    std::cout << "=== 专家：条件类型、common_type、与推导实证三件套 ===\n";
    {
        using C = std::conditional_t<true, int, double>;
        using D = std::conditional_t<false, int, double>;
        static_assert(std::is_same_v<C, int>);
        static_assert(std::is_same_v<D, double>);

        using Common = std::common_type_t<int, double>;
        static_assert(std::is_same_v<Common, double>);

        // enable_if 风格（现代更常用 requires）：仅当可平凡复制时启用
        static_assert(std::is_trivially_copyable_v<int>);
        static_assert(!std::is_trivially_copyable_v<std::string>);

        // 实证工具回顾：static_assert(is_same_v) > 编译错误显示类型 > typeid
        int x = 0;
        auto y = x;
        static_assert(std::is_same_v<decltype(y), int>);

        // is_same 对引用/const 敏感（这正是我们要的）
        static_assert(!std::is_same_v<int, int&>);
        static_assert(!std::is_same_v<int, const int>);

        std::cout << "[expert] traits are the type-level standard library\n";
    }

    std::cout << "[type_traits_intro] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/type_traits_intro", run>;

}  // namespace
