// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : consteval_propagation_immediate_escalating_cpp23
// Topic id : part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23
//
// Refs:
//   P2564R3 — consteval needs to propagate up
//   https://en.cppreference.com/w/cpp/language/consteval
//   https://wg21.link/P2564R3

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// ---------------------------------------------------------------------------
// §入门 — C++20 的「颜色问题」
// ---------------------------------------------------------------------------
// consteval 函数只能在立即函数上下文中调用。把 consteval 回调丢进非
// consteval 的泛型算法 / 普通 lambda 时，C++20 经常直接编译失败。
// P2564 (C++23，并作为 DR 影响 C++20 模式) 引入 immediate-escalating：
// 某些表达式会把 *所在函数* 升级为立即函数。

consteval int double_it(int x) {
    return x * 2;
}

consteval int add_one(int x) {
    return x + 1;
}

// ---------------------------------------------------------------------------
// §进阶 — lambda / 模板被升级为立即函数
// ---------------------------------------------------------------------------
// 未显式标 constexpr 的 lambda：调用 consteval → call operator 升级为立即。
// 升级后的 lambda *只能* 在编译期调用（这正是传播「颜色」的代价与收益）。

constexpr auto make_escalating_lambda() {
    return [](int x) { return double_it(x); };
}

template <class F>
constexpr int apply_if_possible(F f, int x) {
    // 若 F::operator() 是立即函数，本函数在 P2564 下也会成为立即函数
    return f(x);
}

// 显式 constexpr 函数 + *常量* 实参直接调 consteval：无需升级
constexpr int call_double_with_constant() {
    return double_it(21);
}

// 错误模式（注释）：
// constexpr int call_double(int x) { return double_it(x); } // 通常非良构

// ---------------------------------------------------------------------------
// §专家 — 升级规则直觉与工程建议
// ---------------------------------------------------------------------------
// Immediate-escalating expressions 包括：调用立即函数等。
// 非显式 constexpr 的 lambda/模板若包含此类表达式，可被升级为立即函数。
// 显式 constexpr 普通函数不会自动升级（已承诺也可运行期）。
// 工程：
//  - 工具函数用 consteval 表达「只许编译期」；
//  - 胶水层用 lambda/模板承接，让 P2564 传播颜色；
//  - 对外 maybe API 用 constexpr + if consteval。

consteval int pipeline(int x) {
    return add_one(double_it(x));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section03/consteval_propagation_immediate_escalating_cpp23 ===\n";

    static_assert(double_it(21) == 42);
    static_assert(pipeline(10) == 21);
    static_assert(call_double_with_constant() == 42);

    std::cout << "[intro] double_it(21)=" << double_it(21) << " pipeline(10)=" << pipeline(10) << '\n';

    // P2564：lambda 升级后仅编译期可调用
    constexpr auto escalated = make_escalating_lambda();
    static_assert(escalated(21) == 42);
    // int r = escalated(argc); // 升级成功后：非良构（argc 运行期）

    constexpr int via_template = apply_if_possible(make_escalating_lambda(), 7);
    static_assert(via_template == 14);

    std::cout << "[advanced] escalated lambda: static_assert(escalated(21)==42) OK; "
                 "runtime call would be ill-formed after P2564 escalation\n";
    std::cout << "[advanced] apply_if_possible(lambda,7) CT=" << via_template << '\n';

    std::cout << "[expert] P2564 lets consteval 'color' propagate into lambdas/"
                 "templates so you can compose; explicit constexpr functions "
                 "do NOT auto-escalate\n";
    std::cout << "[expert] call_double_with_constant()=" << call_double_with_constant()
              << " (constexpr fn + constant args, no escalation needed)\n";

    assert(pipeline(3) == 7);
    std::cout << "consteval_propagation_immediate_escalating_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23", run>;

}  // namespace
