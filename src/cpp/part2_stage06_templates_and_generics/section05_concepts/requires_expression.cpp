// Topic    : requires 表达式 —— 四类 requirement
// Doc      : 第2部分-阶段6 · 步骤 11.2–11.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : requires_expression
// Topic id : part2/stage06/section05/requires_expression
// Refs     : https://en.cppreference.com/w/cpp/language/requires
//            ISO [expr.prim.req]

#include "learn/topic_registry.hpp"

#include <concepts>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — requires-expression 是返回 bool 的表达式
// ---------------------------------------------------------------------------

template <typename T>
constexpr bool can_preinc = requires(T x) { ++x; };

// ---------------------------------------------------------------------------
// §进阶 — 四类要求：简单 / 类型 / 复合 / 嵌套
// ---------------------------------------------------------------------------

template <typename T>
concept Container = requires(T c) {
    // ① 简单要求：表达式合法
    c.size();
    c.begin();
    c.end();
    // ② 类型要求：嵌套类型存在
    typename T::value_type;
    // ③ 复合要求：合法且结果满足约束
    { c.size() } -> std::convertible_to<std::size_t>;
    { c.begin() } -> std::input_or_output_iterator;
    // ④ 嵌套要求：另一约束
    requires std::copyable<typename T::value_type>;
};

template <typename T>
concept EqualityComparable = requires(const T& a, const T& b) {
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

// ---------------------------------------------------------------------------
// §专家 — 在 if constexpr / 局部断言中使用；参数只作类型检查
// ---------------------------------------------------------------------------

template <typename T>
concept Indexable = requires(T c, std::size_t i) {
    { c[i] } -> std::convertible_to<typename T::value_type>;
};

template <typename T>
auto element_count(const T& c) {
    if constexpr (requires { c.size(); }) {
        return c.size();
    } else {
        return std::size_t{1};
    }
}

struct RawPair {
    int a, b;
    // 无 size / value_type
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [requires_expression] 入门 ===\n";
    static_assert(can_preinc<int>);
    static_assert(!can_preinc<std::string>);  // string 无 ++s 的经典用法
    // 注意：requires 参数不求值，只做语法/语义合法性检查
    std::cout << "can_preinc OK\n";

    std::cout << "=== 进阶：四类要求组成 Container ===\n";
    static_assert(Container<std::vector<int>>);
    static_assert(Container<std::string>);
    static_assert(!Container<RawPair>);
    static_assert(EqualityComparable<int>);
    static_assert(EqualityComparable<std::string>);
    assert(element_count(std::vector{1, 2, 3, 4}) == 4);
    assert(element_count(42) == 1);
    std::cout << "Container concept OK\n";

    std::cout << "=== 专家：Indexable + 复合要求箭头 ===\n";
    static_assert(Indexable<std::vector<int>>);
    static_assert(Indexable<std::string>);
    // requires 里的 c、i 是虚构变量，不产生运行期对象
    std::vector<int> v{10, 20};
    assert(v[0] == 10);
    // 复合要求 { expr } -> Concept 等价于
    //   requires { expr; } && Concept<decltype((expr))>
    std::cout << "=== requires_expression: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/requires_expression", run>;

}  // namespace
