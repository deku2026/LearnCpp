// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E3 Concepts 深水)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : concepts_versus_sfinae
// Topic id : part6/e/section03/concepts_versus_sfinae
//
// 要点: Concepts 是受约束模板的一等公民 —— 可读诊断 + 偏序;
//       SFINAE 仍可用但错误信息差、写法冗长。
// 参考: [temp.constr] cppreference constraints and concepts

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <iostream>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// --- SFINAE 风格 ---
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> twice_sfinae(T x) {
    return static_cast<T>(x * 2);
}

// --- Concepts 风格 ---
template <std::integral T>
T twice_concept(T x) {
    return static_cast<T>(x * 2);
}

// 自定义 concept vs enable_if 组合
template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <Addable T>
T add_c(T a, T b) {
    return a + b;
}

template <typename T>
std::enable_if_t<std::is_arithmetic_v<T>, T> add_s(T a, T b) {
    return a + b;
}

// 重载选择清晰度
template <typename T>
std::string describe_sfinae(T, std::enable_if_t<std::is_pointer_v<T>, int> = 0) {
    return "pointer(sfinae)";
}

template <typename T>
std::string describe_sfinae(T, std::enable_if_t<!std::is_pointer_v<T>, int> = 0) {
    return "value(sfinae)";
}

template <typename T>
std::string describe_c(T*) {
    return "pointer(concept-overload)";
}

template <typename T>
std::string describe_c(T) {
    return "value(concept-overload)";
}

// Concepts 可直接用于 auto 形参 (简写)
std::string size_label(const std::ranges::sized_range auto& r) {
    return "sized:" + std::to_string(std::ranges::size(r));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E3 concepts versus SFINAE ===\n";

    assert(twice_sfinae(21) == 42);
    assert(twice_concept(21) == 42);
    assert(add_c(3, 4) == 7);
    assert(add_s(3.5, 0.5) == 4.0);

    int x = 1;
    assert(describe_sfinae(&x) == "pointer(sfinae)");
    assert(describe_sfinae(x) == "value(sfinae)");
    assert(describe_c(&x) == "pointer(concept-overload)");
    assert(describe_c(x) == "value(concept-overload)");

    std::vector<int> v{1, 2, 3};
    assert(size_label(v) == "sized:3");

    std::cout << "  concepts: better diagnostics, subsumption, terse syntax\n";
    std::cout << "  SFINAE: still in pre-C++20 libraries; interop via enable_if\n";
    // 不匹配时 concepts 报 "constraints not satisfied", SFINAE 常刷屏 substitution
    std::cout << "concepts_versus_sfinae: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/concepts_versus_sfinae", run>;

}  // namespace
