// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E3 Concepts 深水)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : requires_expression_clauses
// Topic id : part6/e/section03/requires_expression_clauses
//
// 要点: requires 表达式四种子句 —— 简单 / 类型 / 复合 / 嵌套;
//       requires 子句 vs requires 表达式 别混。
// 参考: [expr.prim.req] cppreference requires expression

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// requires 表达式 → 编译期 bool
template <typename T>
concept Incrementable = requires(T t) {
    // 简单要求: 表达式合法即可
    ++t;
    t++;
};

template <typename T>
concept HasValueType = requires {
    // 类型要求
    typename T::value_type;
};

template <typename T>
concept Hashable = requires(T a) {
    // 复合要求: 合法 + 返回类型约束
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept Containerish = requires(T c) {
    typename T::value_type;
    { c.begin() } -> std::input_or_output_iterator;
    { c.end() } -> std::input_or_output_iterator;
    // 嵌套要求
    requires std::default_initializable<typename T::value_type>;
    requires sizeof(typename T::value_type) > 0;
};

// requires 子句 (约束写在模板头)
template <typename T>
    requires Incrementable<T> && std::copyable<T>
T next_copy(T x) {
    ++x;
    return x;
}

// 简写 + 尾随 requires
template <typename T>
auto value_name(const T& t)
    requires HasValueType<T>
{
    return typeid(typename T::value_type).name();
}

// 局部 requires 表达式作为常量
template <typename T>
constexpr bool can_add = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

struct Counter {
    int n = 0;
    Counter& operator++() {
        ++n;
        return *this;
    }
    Counter operator++(int) {
        Counter t = *this;
        ++*this;
        return t;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E3 requires expression clauses ===\n";

    static_assert(Incrementable<int>);
    static_assert(Incrementable<Counter>);
    static_assert(HasValueType<std::vector<int>>);
    static_assert(!HasValueType<int>);
    static_assert(Hashable<std::string>);
    static_assert(Containerish<std::vector<int>>);
    static_assert(can_add<int>);
    static_assert(!can_add<std::vector<int>>);

    assert(next_copy(10) == 11);
    Counter c{};
    [[maybe_unused]] auto c2 = next_copy(c);
    assert(c2.n == 1);

    std::vector<int> v;
    (void)value_name(v);
    std::cout << "  simple/type/compound/nested requirements demonstrated\n";
    std::cout << "  clause constrains template; expression is a bool predicate\n";
    std::cout << "requires_expression_clauses: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/requires_expression_clauses", run>;

}  // namespace
