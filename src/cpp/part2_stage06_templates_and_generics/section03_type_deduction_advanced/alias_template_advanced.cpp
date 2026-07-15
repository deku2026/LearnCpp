// Topic    : 别名模板 —— 简化复杂类型；不能偏特化
// Doc      : 第2部分-阶段6 · 步骤 6
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : alias_template_advanced
// Topic id : part2/stage06/section03/alias_template_advanced
// Refs     : https://en.cppreference.com/w/cpp/language/type_alias
//            ISO [temp.alias]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 给冗长类型起短名
// ---------------------------------------------------------------------------

template <typename T>
using Vec = std::vector<T>;

template <typename V>
using StringMap = std::map<std::string, V>;

// ---------------------------------------------------------------------------
// §进阶 — trait 的 _t 后缀就是别名模板
// ---------------------------------------------------------------------------

template <typename T>
using RemoveRef = typename std::remove_reference<T>::type;  // C++11 风格

// 等价于 std::remove_reference_t（C++14）
template <bool B, typename T = void>
using EnableIf = typename std::enable_if<B, T>::type;

template <typename T>
EnableIf<std::is_integral_v<T>, T> twice(T x) {
    return static_cast<T>(x * 2);
}

// ---------------------------------------------------------------------------
// §专家 — 别名不能偏特化：底层用类模板 + 表层别名
// ---------------------------------------------------------------------------

// ❌ 非法：template<typename T> using X = ...; template<typename T> using X<T*> = ...
// ✅ 合法：类模板偏特化，再 using 取 ::type

template <typename T>
struct PointeeImpl {
    using type = T;
};

template <typename T>
struct PointeeImpl<T*> {
    using type = T;
};

template <typename T>
using Pointee = typename PointeeImpl<T>::type;

// 可变参数别名
template <typename... Ts>
using LastPtrTuple = std::tuple<Ts*...>;

// 高阶：别名模板作「元函数」传递（配合模板模板参数）
template <template <typename> class Meta, typename T>
using Invoke = Meta<T>;

template <typename T>
using AddConst = const T;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [alias_template_advanced] 入门 ===\n";
    Vec<int> v{1, 2, 3};
    assert(v.size() == 3);
    StringMap<int> ages{{"Ada", 36}, {"Bob", 28}};
    assert(ages["Ada"] == 36);
    static_assert(std::is_same_v<Vec<int>, std::vector<int>>);
    std::cout << "Vec / StringMap OK\n";

    std::cout << "=== 进阶：_t 风格别名与 EnableIf ===\n";
    static_assert(std::is_same_v<RemoveRef<int&>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<const int&&>, const int>);
    assert(twice(21) == 42);
    // twice(1.5); // SFINAE 禁用
    std::cout << "twice(21)=" << twice(21) << '\n';

    std::cout << "=== 专家：偏特化放类模板，别名只做表层 ===\n";
    static_assert(std::is_same_v<Pointee<int**>, int*>);  // 只剥一层（本实现）
    static_assert(std::is_same_v<Pointee<int*>, int>);
    static_assert(std::is_same_v<Pointee<int>, int>);
    using P = LastPtrTuple<int, double>;
    static_assert(std::is_same_v<std::tuple_element_t<1, P>, double*>);
    static_assert(std::is_same_v<Invoke<AddConst, int>, const int>);
    std::cout << "=== alias_template_advanced: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/alias_template_advanced", run>;

}  // namespace
