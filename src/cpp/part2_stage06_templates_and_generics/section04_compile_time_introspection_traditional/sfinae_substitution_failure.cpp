// Topic    : SFINAE —— 替换失败非错误；直接上下文 vs 硬错误
// Doc      : 第2部分-阶段6 · 步骤 9.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : sfinae_substitution_failure
// Topic id : part2/stage06/section04/sfinae_substitution_failure
// Refs     : https://en.cppreference.com/w/cpp/language/sfinae
//            ISO [temp.deduct]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 替换失败只移除候选，不是硬编译错误
// ---------------------------------------------------------------------------

// 仅当 T 有 nested size_type 时此重载参与
template <typename T>
auto size_of_container(const T& c) -> typename T::size_type {
    return c.size();
}

// 回退：原始数组
template <typename T, std::size_t N>
std::size_t size_of_container(const T (&)[N]) {
    return N;
}

// ---------------------------------------------------------------------------
// §进阶 — 用表达式 SFINAE 检测可调用
// ---------------------------------------------------------------------------

template <typename T>
auto serialize_impl(const T& x, int) -> decltype(void(x.serialize()), std::string{}) {
    return x.serialize();
}

template <typename T>
std::string serialize_impl(const T& x, long) {
    return std::to_string(x);
}

template <typename T>
std::string serialize(const T& x) {
    return serialize_impl(x, 0);  // 优先匹配 int 版（更佳转换）
}

struct Jsonish {
    std::string serialize() const { return "{}"; }
};

// ---------------------------------------------------------------------------
// §专家 — 直接上下文；与 enable_if 协作；失败不污染重载集
// ---------------------------------------------------------------------------

template <typename T>
auto deref_if_pointer(T p) -> std::enable_if_t<std::is_pointer_v<T>, std::remove_pointer_t<T>> {
    return *p;
}

template <typename T>
auto deref_if_pointer(T v) -> std::enable_if_t<!std::is_pointer_v<T>, T> {
    return v;
}

// 仅对有 value_type 的类型启用
template <typename T, typename = typename T::value_type>
constexpr bool has_value_type_param(int) {
    return true;
}
template <typename T>
constexpr bool has_value_type_param(long) {
    return false;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [sfinae] 入门：容器 size 重载 ===\n";
    std::vector<int> v{1, 2, 3, 4};
    assert(size_of_container(v) == 4);
    int arr[3]{};
    assert(size_of_container(arr) == 3);
    // size_of_container(3) → 两候选都替换失败/不匹配 → 硬错误（无可行函数）
    std::cout << "vector/array size OK\n";

    std::cout << "=== 进阶：表达式 SFINAE 序列化 ===\n";
    assert(serialize(Jsonish{}) == "{}");
    assert(serialize(42) == "42");
    std::cout << "serialize polymorphic via SFINAE OK\n";

    std::cout << "=== 专家：直接上下文与探测 ===\n";
    int x = 7;
    int* px = &x;
    assert(deref_if_pointer(px) == 7);
    assert(deref_if_pointer(9) == 9);
    static_assert(has_value_type_param<std::vector<int>>(0));
    static_assert(!has_value_type_param<int>(0));
    // 关键：失败必须在「函数类型/模板参数」的直接上下文；
    // 若失败发生在函数体实例化或深层副作用，会变成硬错误而非 SFINAE。
    std::cout << "=== sfinae_substitution_failure: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/sfinae_substitution_failure", run>;

}  // namespace
