// Topic    : type_traits —— is_same / 谓词 / 变换全家桶入门
// Doc      : 第2部分-阶段6 · 步骤 9.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : type_traits_is_same_and_friends
// Topic id : part2/stage06/section04/type_traits_is_same_and_friends
// Refs     : https://en.cppreference.com/w/cpp/header/type_traits
//            ISO [meta]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 谓词 _v 与变换 _t
// ---------------------------------------------------------------------------

// 手写 is_same（理解 trait 本质：主模板 + 全特化）
template <typename T, typename U>
struct MyIsSame : std::false_type {};

template <typename T>
struct MyIsSame<T, T> : std::true_type {};

// ---------------------------------------------------------------------------
// §进阶 — 组合查询；conditional；decay
// ---------------------------------------------------------------------------

template <typename T>
using NoCvRef = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
constexpr bool is_stringy_v = std::is_same_v<NoCvRef<T>, std::string> || std::is_same_v<NoCvRef<T>, const char*> ||
                              std::is_same_v<NoCvRef<T>, char*>;

// ---------------------------------------------------------------------------
// §专家 — 手写 remove_reference / is_pointer；与标准对照
// ---------------------------------------------------------------------------

template <typename T>
struct MyRemoveReference {
    using type = T;
};
template <typename T>
struct MyRemoveReference<T&> {
    using type = T;
};
template <typename T>
struct MyRemoveReference<T&&> {
    using type = T;
};
template <typename T>
using MyRemoveReference_t = typename MyRemoveReference<T>::type;

template <typename T>
struct MyIsPointer : std::false_type {};
template <typename T>
struct MyIsPointer<T*> : std::true_type {};

template <typename T>
std::string category() {
    if constexpr (std::is_integral_v<T>) {
        return "integral";
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating";
    } else if constexpr (std::is_pointer_v<T>) {
        return "pointer";
    } else if constexpr (std::is_class_v<T>) {
        return "class";
    } else {
        return "other";
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [type_traits] 入门：is_same 与常用谓词 ===\n";
    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, int&>);
    static_assert(MyIsSame<int, int>::value);
    static_assert(!MyIsSame<int, double>::value);
    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(!std::is_pointer_v<int>);
    static_assert(std::is_pointer_v<int*>);
    std::cout << "predicates OK\n";

    std::cout << "=== 进阶：变换与 conditional ===\n";
    static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
    static_assert(std::is_same_v<std::conditional_t<true, int, double>, int>);
    static_assert(std::is_same_v<std::conditional_t<false, int, double>, double>);
    static_assert(is_stringy_v<std::string>);
    static_assert(is_stringy_v<const char*>);
    static_assert(!is_stringy_v<int>);
    using A = std::common_type_t<int, double>;
    static_assert(std::is_same_v<A, double>);
    std::cout << "transforms OK\n";

    std::cout << "=== 专家：手写 trait 对齐标准 ===\n";
    static_assert(std::is_same_v<MyRemoveReference_t<int&&>, int>);
    static_assert(MyIsPointer<int*>::value);
    static_assert(!MyIsPointer<int>::value);
    assert(category<int>() == "integral");
    assert(category<double>() == "floating");
    assert(category<std::vector<int>>() == "class");
    assert(category<int*>() == "pointer");
    // is_base_of / is_constructible 等也是编译期 API
    static_assert(std::is_constructible_v<std::string, const char*>);
    static_assert(std::is_nothrow_move_constructible_v<std::string>);
    std::cout << "=== type_traits_is_same_and_friends: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/type_traits_is_same_and_friends", run>;

}  // namespace
