// Topic    : concept 定义 —— 具名编译期谓词
// Doc      : 第2部分-阶段6 · 步骤 11.1
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : concept_definition
// Topic id : part2/stage06/section05/concept_definition
// Refs     : https://en.cppreference.com/w/cpp/language/constraints
//            ISO [temp.constr] / [concepts]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 用 type_traits 定义 concept
// ---------------------------------------------------------------------------

template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
concept Floating = std::is_floating_point_v<T>;

// ---------------------------------------------------------------------------
// §进阶 — requires 表达式定义「能力」
// ---------------------------------------------------------------------------

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <typename T>
concept HasSize = requires(const T& c) {
    { c.size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept Printable = requires(std::ostream& os, const T& x) { os << x; };

// ---------------------------------------------------------------------------
// §专家 — concept 可组合；可作为 bool 常量使用
// ---------------------------------------------------------------------------

template <typename T>
concept Numeric = Integral<T> || Floating<T>;

template <typename T>
concept NumberLike = Numeric<T> && Addable<T>;

template <NumberLike T>
T add_num(T a, T b) {
    return a + b;
}

template <HasSize T>
std::size_t len(const T& c) {
    return static_cast<std::size_t>(c.size());
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [concept_definition] 入门 ===\n";
    static_assert(Integral<int>);
    static_assert(!Integral<double>);
    static_assert(Floating<float>);
    // concept 名本身是编译期谓词
    static_assert(Integral<char> == true);
    std::cout << "Integral/Floating OK\n";

    std::cout << "=== 进阶：能力型 concept ===\n";
    static_assert(Addable<int>);
    static_assert(Addable<std::string>);
    static_assert(HasSize<std::vector<int>>);
    static_assert(HasSize<std::string>);
    static_assert(Printable<int>);
    assert(len(std::string{"abcd"}) == 4);
    assert(len(std::vector<int>{1, 2, 3}) == 3);
    std::cout << "Addable/HasSize/Printable OK\n";

    std::cout << "=== 专家：组合 concept ===\n";
    static_assert(NumberLike<int>);
    static_assert(NumberLike<double>);
    static_assert(!NumberLike<std::string>);  // 非 Numeric
    assert(add_num(2, 3) == 5);
    assert(add_num(1.5, 2.5) == 4.0);
    // add_num(std::string{"a"}, std::string{"b"}); // 约束失败，报错清晰
    std::cout << "=== concept_definition: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/concept_definition", run>;

}  // namespace
