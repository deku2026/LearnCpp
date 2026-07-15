// Topic    : 模板实参推导 —— 按值/引用/转发引用与非推导上下文
// Doc      : 第2部分-阶段6 · 步骤 1.2 / 4
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : template_argument_deduction
// Topic id : part2/stage06/section01/template_argument_deduction
// Refs     : https://en.cppreference.com/w/cpp/language/template_argument_deduction
//            Effective Modern C++ Item 1
//            ISO [temp.deduct.call]

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 三种基本模式（与 auto 同源）
// ---------------------------------------------------------------------------

template <typename T>
std::string describe_by_value(T) {
    if constexpr (std::is_pointer_v<T>) {
        return "pointer";
    } else if constexpr (std::is_array_v<T>) {
        return "array";  // 按值实际会退化，通常走不到
    } else if constexpr (std::is_const_v<T>) {
        return "const-value";
    } else {
        return "value";
    }
}

template <typename T>
using strip = std::remove_cvref_t<T>;

template <typename T>
constexpr bool is_const_ref() {
    return std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>;
}

// 用类型特征把推导结果「打印」成编译期可断言的标签
template <typename T>
struct Deduced {
    using type = T;
};

template <typename T>
Deduced<T> probe_value(T) {
    return {};
}

template <typename T>
Deduced<T> probe_ref(T&) {
    return {};
}

template <typename T>
Deduced<T> probe_fwd(T&&) {
    return {};
}

// ---------------------------------------------------------------------------
// §进阶 — 数组退化 vs 引用保留维度；函数指针
// ---------------------------------------------------------------------------

template <typename T, std::size_t N>
constexpr std::size_t array_extent(const T (&)[N]) {
    return N;
}

template <typename T>
constexpr bool is_pointer_param(T) {
    return std::is_pointer_v<T>;
}

// ---------------------------------------------------------------------------
// §专家 — 非推导上下文；返回类型不参与推导
// ---------------------------------------------------------------------------

template <typename T>
struct Identity {
    using type = T;
};

// Identity<T>::type 中的 T 处于非推导上下文
template <typename T>
void take_identity(typename Identity<T>::type) {}

template <typename T>
T make_default() {
    return T{};
}

// 部分推导：显式指定一部分，其余从实参推导
template <typename Out, typename In>
Out narrow_cast(In x) {
    return static_cast<Out>(x);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [template_argument_deduction] 入门：三种模式 ===\n";
    const int ci = 42;
    {
        auto v = probe_value(ci);
        static_assert(std::is_same_v<decltype(v)::type, int>);  // 丢 const
    }
    {
        auto r = probe_ref(ci);
        static_assert(std::is_same_v<decltype(r)::type, const int>);  // 保留
    }
    {
        auto fl = probe_fwd(ci);  // 左值 → T = const int&
        static_assert(std::is_same_v<decltype(fl)::type, const int&>);
        auto fr = probe_fwd(42);  // 右值 → T = int
        static_assert(std::is_same_v<decltype(fr)::type, int>);
    }
    std::cout << "by-value / by-ref / forwarding-ref OK\n";

    std::cout << "=== 进阶：数组退化 vs T(&)[N] ===\n";
    int arr[5]{};
    auto decayed = probe_value(arr);
    static_assert(std::is_same_v<decltype(decayed)::type, int*>);
    assert(is_pointer_param(arr));
    assert(array_extent(arr) == 5);
    // 引用绑定保留数组类型
    auto kept = probe_ref(arr);
    static_assert(std::is_same_v<decltype(kept)::type, int[5]>);
    std::cout << "array extent=" << array_extent(arr) << '\n';

    std::cout << "=== 专家：非推导上下文 / 显式部分指定 ===\n";
    // take_identity(1) 无法推导 T，必须 take_identity<int>(1)
    take_identity<int>(1);
    assert(make_default<std::string>().empty());
    assert(narrow_cast<int>(3.9) == 3);
    // ⚠️ larger(3, 2.5) 单 T 失败；推导要求实参模式一致
    // describe 只是演示按值标签
    assert(describe_by_value(1) == "value");
    assert(describe_by_value(&ci) == "pointer");
    static_assert(is_const_ref<const int&>());
    std::cout << "=== template_argument_deduction: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/template_argument_deduction", run>;

}  // namespace
