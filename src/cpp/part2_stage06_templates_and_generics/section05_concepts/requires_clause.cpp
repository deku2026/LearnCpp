// Topic    : requires 子句 —— 给模板施加约束的四种写法
// Doc      : 第2部分-阶段6 · 步骤 11.2 / 11.5
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : requires_clause
// Topic id : part2/stage06/section05/requires_clause
// Refs     : https://en.cppreference.com/w/cpp/language/constraints
//            ISO [temp.pre] / [temp.constr]

#include "learn/topic_registry.hpp"

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — requires 子句 vs requires 表达式
// ---------------------------------------------------------------------------

template <typename T>
concept Incrementable = requires(T x) {  // 这是 requires-expression
    ++x;
};

// requires-clause：跟在模板参数后
template <typename T>
    requires Incrementable<T>
T bump(T x) {
    ++x;
    return x;
}

// ---------------------------------------------------------------------------
// §进阶 — 四种等价施加方式
// ---------------------------------------------------------------------------

// 1) 类型约束简写
template <std::integral T>
T add1(T a, T b) {
    return a + b;
}

// 2) 模板头后 requires 子句
template <typename T>
    requires std::integral<T>
T add2(T a, T b) {
    return a + b;
}

// 3) 尾置 requires
template <typename T>
T add3(T a, T b)
    requires std::integral<T>
{
    return a + b;
}

// 4) constrained auto（简写函数模板）
auto add4(std::integral auto a, std::integral auto b) {
    return a + b;
}

// ---------------------------------------------------------------------------
// §专家 — 复合约束；requires requires 反模式
// ---------------------------------------------------------------------------

template <typename T>
    requires std::integral<T> && (sizeof(T) >= 4)
T wide_id(T x) {
    return x;
}

// 合法但丑：requires requires(...)
template <typename T>
    requires requires(T a, T b) { a + b; }
T ugly_add(T a, T b) {
    return a + b;
}

// 推荐：具名 concept
template <typename T>
concept Summable = requires(T a, T b) { a + b; };

template <typename T>
    requires Summable<T>
T nice_add(T a, T b) {
    return a + b;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [requires_clause] 入门 ===\n";
    assert(bump(10) == 11);
    // bump(std::string{"x"}); // 不满足 Incrementable
    static_assert(Incrementable<int>);
    std::cout << "bump(10)=" << bump(10) << '\n';

    std::cout << "=== 进阶：四种写法等价 ===\n";
    assert(add1(1, 2) == 3);
    assert(add2(1, 2) == 3);
    assert(add3(1, 2) == 3);
    assert(add4(1, 2) == 3);
    // add4(1.0, 2.0); // 约束失败
    std::cout << "add1..add4 OK\n";

    std::cout << "=== 专家：复合约束与具名化 ===\n";
    assert(wide_id(100) == 100);
    // wide_id(short{1}); // sizeof < 4 可能失败
    assert(ugly_add(2, 3) == 5);
    assert(nice_add(std::string{"a"}, std::string{"b"}) == "ab");
    static_assert(Summable<int>);
    std::cout << "prefer named concepts over requires requires\n";
    std::cout << "=== requires_clause: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/requires_clause", run>;

}  // namespace
