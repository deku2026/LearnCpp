// Topic    : 模板实参推导：与 auto 同源；转发引用特例
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 6.1 / 4.5
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : template_argument_deduction
// Topic id : part2/stage02/section06/template_argument_deduction
// Refs     : https://en.cppreference.com/cpp/language/template_argument_deduction
//            https://en.cppreference.com/cpp/language/auto
//            ISO [temp.deduct.call]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// 按值参数：剥顶层 const / 引用（与 auto x = expr 同规则）
template <class T>
constexpr bool deduce_by_value(T) {
    return std::is_same_v<T, int>;  // 调用方用 static 场景外运行期检查类型特征
}

template <class T>
struct TypeOf;

// 用返回类型暴露推导结果
template <class T>
constexpr T by_value_id(T x) {
    return x;
}

template <class T>
constexpr T& by_lref_id(T& x) {
    return x;
}

template <class T>
constexpr T&& by_fwd_id(T&& x) {
    return std::forward<T>(x);
}

// 数组 / 函数按值会退化
template <class T>
constexpr bool is_ptr_param(T) {
    return std::is_pointer_v<T>;
}

// 非转发：vector<T>&& 是右值引用，不是 forwarding reference
template <class T>
void only_rvalue_vector(std::vector<T>&&) {}

// const T&& 也不是转发引用
template <class T>
void const_rvalue_only(const T&&) {}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [template_argument_deduction] 入门：按值推导与 auto 同源 ===\n";
    {
        int a = 1;
        const int ca = 2;
        int& ra = a;
        const int& cra = ca;

        // template<class T> void f(T); 对上述实参 T 均为 int
        static_assert(std::is_same_v<decltype(by_value_id(a)), int>);
        static_assert(std::is_same_v<decltype(by_value_id(ca)), int>);
        static_assert(std::is_same_v<decltype(by_value_id(ra)), int>);
        static_assert(std::is_same_v<decltype(by_value_id(cra)), int>);

        // 对照 auto
        auto xa = a;
        auto xca = ca;
        static_assert(std::is_same_v<decltype(xa), int>);
        static_assert(std::is_same_v<decltype(xca), int>);

        assert(by_value_id(cra) == 2);
        std::cout << "[intro] by-value template param strips like auto\n";
    }

    std::cout << "=== 进阶：引用参数；数组退化；初始化列表不推导 ===\n";
    {
        int a = 3;
        const int ca = 4;

        // template<class T> void f(T&); → 保留 const
        static_assert(std::is_same_v<decltype(by_lref_id(a)), int&>);
        static_assert(std::is_same_v<decltype(by_lref_id(ca)), const int&>);

        [[maybe_unused]] int arr[4]{1, 2, 3, 4};
        assert(is_ptr_param(arr));  // T = int*
        // 若写成 T (&)[N] 则保留数组

        // 注意：f({1,2,3}) 通常无法推导 T（initializer_list 特例不在这里）
        // 需要 template<class T> void f(std::initializer_list<T>);

        std::cout << "[advanced] T& keeps const; arrays decay when by value\n";
    }

    std::cout << "=== 专家：转发引用推导；哪些 && 不是转发 ===\n";
    {
        int a = 5;
        // 左值 → T = int&；右值 → T = int
        decltype(auto) r1 = by_fwd_id(a);
        decltype(auto) r2 = by_fwd_id(6);
        static_assert(std::is_same_v<decltype(r1), int&>);
        static_assert(std::is_same_v<decltype(r2), int&&>);
        assert(&r1 == &a);

        std::vector<int> v{1, 2};
        only_rvalue_vector(std::move(v));
        // only_rvalue_vector(v);  // ❌ 不是转发引用，不收左值

        const int c = 0;
        const_rvalue_only(std::move(c));
        // const_rvalue_only(c);  // ❌ 左值不匹配 const T&&

        // 判定转发引用：① 形式为未限定的 T&&；② T 正在被推导（或 auto&&）
        std::cout << "[expert] T&& is forwarding only if bare T is deduced\n";
    }

    std::cout << "[template_argument_deduction] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/template_argument_deduction", run>;

}  // namespace
