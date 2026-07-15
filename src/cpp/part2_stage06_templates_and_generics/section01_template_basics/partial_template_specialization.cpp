// Topic    : 偏特化 —— 匹配一类模式（仅类模板）；函数用重载
// Doc      : 第2部分-阶段6 · 步骤 5.2–5.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : partial_template_specialization
// Topic id : part2/stage06/section01/partial_template_specialization
// Refs     : https://en.cppreference.com/w/cpp/language/partial_specialization
//            ISO [temp.class.spec]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// §入门 — IsPointer：主模板 vs T* 偏特化
// ---------------------------------------------------------------------------

template <typename T>
struct IsPointer : std::false_type {};

template <typename T>
struct IsPointer<T*> : std::true_type {};

// ---------------------------------------------------------------------------
// §进阶 — 多参数模式：pair 第二参数、指针到成员、const 剥离组合
// ---------------------------------------------------------------------------

template <typename T>
struct Rank {
    static constexpr int value = 0;
};

template <typename T>
struct Rank<T*> {
    static constexpr int value = 1 + Rank<T>::value;
};

template <typename T>
struct Rank<T[]> {
    static constexpr int value = 10 + Rank<T>::value;
};

template <typename T, std::size_t N>
struct Rank<T[N]> {
    static constexpr int value = 100 + Rank<T>::value;
};

// 键值对：偏特化「值是指针」的地图条目元数据
template <typename K, typename V>
struct EntryInfo {
    static constexpr bool value_is_pointer = false;
    using key_type = K;
    using value_type = V;
};

template <typename K, typename V>
struct EntryInfo<K, V*> {
    static constexpr bool value_is_pointer = true;
    using key_type = K;
    using value_type = V*;
    using pointee = V;
};

// ---------------------------------------------------------------------------
// §专家 — 函数不能偏特化 → 用重载；偏特化选择更「特殊」的匹配
// ---------------------------------------------------------------------------

template <typename T>
void process(T) {
    // generic
}

template <typename T>
void process(T*) {
    // overload for any pointer — NOT partial specialization of function
}

template <typename T>
struct Select {
    static constexpr const char* tag() { return "primary"; }
};

template <typename T>
struct Select<const T> {
    static constexpr const char* tag() { return "const"; }
};

template <typename T>
struct Select<T*> {
    static constexpr const char* tag() { return "pointer"; }
};

template <typename T>
struct Select<const T*> {
    static constexpr const char* tag() { return "const-pointer"; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [partial_template_specialization] 入门：IsPointer ===\n";
    static_assert(!IsPointer<int>::value);
    static_assert(IsPointer<int*>::value);
    static_assert(IsPointer<double*>::value);
    static_assert(!IsPointer<int&>::value);
    std::cout << "IsPointer<int*>=" << IsPointer<int*>::value << '\n';

    std::cout << "=== 进阶：Rank 递归偏特化 / EntryInfo ===\n";
    static_assert(Rank<int>::value == 0);
    static_assert(Rank<int**>::value == 2);
    static_assert(Rank<int[3]>::value == 100);
    static_assert(Rank<int[]>::value == 10);
    static_assert(!EntryInfo<int, double>::value_is_pointer);
    static_assert(EntryInfo<int, double*>::value_is_pointer);
    static_assert(std::is_same_v<EntryInfo<int, double*>::pointee, double>);
    std::cout << "Rank<int**>=" << Rank<int**>::value << '\n';

    std::cout << "=== 专家：偏特化偏序；函数用重载 ===\n";
    assert(std::string{Select<int>::tag()} == "primary");
    assert(std::string{Select<const int>::tag()} == "const");
    assert(std::string{Select<int*>::tag()} == "pointer");
    // const int* 同时匹配 T* 与 const T* → 更特殊的 const T* 胜出
    assert(std::string{Select<const int*>::tag()} == "const-pointer");

    int x = 0;
    int* px = &x;
    process(x);   // generic
    process(px);  // pointer overload
    // ⚠️ template<typename T> void process<T*>(...)  非法：函数不能偏特化
    std::cout << "=== partial_template_specialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/partial_template_specialization", run>;

}  // namespace
