// Topic    : 用 Concepts 取代 SFINAE —— 可读性与报错质量
// Doc      : 第2部分-阶段6 · 步骤 11.6
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : replace_sfinae_with_concepts
// Topic id : part2/stage06/section05/replace_sfinae_with_concepts
// Refs     : https://en.cppreference.com/w/cpp/language/constraints
//            https://en.cppreference.com/w/cpp/types/enable_if

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 同一需求的 SFINAE 版 vs Concepts 版
// ---------------------------------------------------------------------------

// --- SFINAE 版 ---
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T process_sfinae(T x) {
    return static_cast<T>(x * 2);
}

// --- Concepts 版 ---
template <std::integral T>
T process_concepts(T x) {
    return static_cast<T>(x * 2);
}

// ---------------------------------------------------------------------------
// §进阶 — 能力约束：has size 的两写法
// ---------------------------------------------------------------------------

template <typename T, typename = void>
struct enable_if_has_size {};

template <typename T>
struct enable_if_has_size<T, std::void_t<decltype(std::declval<const T&>().size())>> {
    using type = void;
};

template <typename T, typename = typename enable_if_has_size<T>::type>
std::size_t size_sfinae(const T& c) {
    return c.size();
}

template <typename T>
    requires requires(const T& c) { c.size(); }
std::size_t size_concepts(const T& c) {
    return c.size();
}

// ---------------------------------------------------------------------------
// §专家 — 多约束重载：Concepts 偏序更自然
// ---------------------------------------------------------------------------

template <typename T>
std::string route(T) {
    return "generic";
}

template <std::integral T>
std::string route(T) {
    return "integral";
}

template <std::floating_point T>
std::string route(T) {
    return "floating";
}

// SFINAE half 双写 vs concept 约束 + if constexpr 混合
template <typename T>
    requires std::integral<T> || std::floating_point<T>
auto half_modern(T x) {
    if constexpr (std::is_integral_v<T>) {
        return x / 2;
    } else {
        return x / 2.0;
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [replace_sfinae_with_concepts] 入门对比 ===\n";
    assert(process_sfinae(21) == 42);
    assert(process_concepts(21) == 42);
    // process_sfinae(1.5);    // 错误：no matching function + 长串 substitution
    // process_concepts(1.5);  // 错误：constraints not satisfied: integral<double>
    std::cout << "both accept int; double would fail (concepts clearer)\n";

    std::cout << "=== 进阶：size 探测 ===\n";
    std::vector<int> v{1, 2, 3};
    assert(size_sfinae(v) == 3);
    assert(size_concepts(v) == 3);
    assert(size_concepts(std::string{"abcd"}) == 4);
    // size_concepts(3); // 直接说明 requires 表达式不满足
    std::cout << "size_* OK\n";

    std::cout << "=== 专家：重载选择与现代写法 ===\n";
    assert(route(1) == "integral");
    assert(route(1.0) == "floating");
    assert(route(std::string{"x"}) == "generic");
    assert(half_modern(7) == 3);
    assert(std::abs(half_modern(7.0) - 3.5) < 1e-12);
    // 迁移策略：新代码 Concepts；维护旧库时保留 enable_if；
    // 函数内分支优先 if constexpr，约束接口优先 concept
    std::cout << "=== replace_sfinae_with_concepts: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/replace_sfinae_with_concepts", run>;

}  // namespace
