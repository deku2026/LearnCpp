// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_function_never_constant_evaluated_cpp23
// Topic id : part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23
//
// Refs:
//   P2448R2 — Relaxing some constexpr restrictions
//   https://en.cppreference.com/w/cpp/language/constexpr
//   Example on cppreference: constexpr void g(int& i) { f(i); } well-formed since C++23
//   even if no invocation can be a core constant expression

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 旧规则：constexpr 函数必须「存在某次调用」能是核心常量表达式
// ---------------------------------------------------------------------------
// C++20 及更早：若编译器能证明 *没有任何* 实参组合能让该函数（或某特化）
// 满足核心常量表达式，则程序非良构（诊断）。
// C++23 P2448：即使永远无法在编译期求值，标 constexpr 也 *不再* 因此非法。
//
// 动机：泛型 / 标准库大量「保守地标 constexpr」；作者不必证明每种 T 都能
// 常量求值。

void mutate_runtime_only(int& i) {
    i = 0;  // 非 constexpr 函数
}

// C++23: 合法 —— 无条件调用非 constexpr 函数，常量求值永不可能成功
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L

constexpr void reset_via_runtime_helper(int& i) {
    mutate_runtime_only(i);
}

constexpr void log_and_touch(std::string& s) {
    // 依赖非 constexpr 设施（iostream 等）的路径会阻止常量求值，
    // 但声明本身在 C++23 可接受。
    s.push_back('!');
}

#else

// 旧工具链：用「至少有一条可常量求值路径」的写法保持可编译
constexpr void reset_via_runtime_helper(int& i) {
    i = 0;
}

constexpr void log_and_touch(std::string& s) {
    s.push_back('!');
}

#endif

// ---------------------------------------------------------------------------
// §进阶 — 对比：仍有可常量求值路径的「真正 maybe constexpr」
// ---------------------------------------------------------------------------

constexpr int maybe_square(int x) {
    // 对任意字面值实参都能常量求值
    return x * x;
}

constexpr int branchy(int x) {
    if (x < 0) {
        // 负值路径若调用非 constexpr，则 *该路径* 不能用于常量求值；
        // 但正值路径仍可，故整体函数在旧规则下也合法。
        return x;  // 保持简单，两条路径都可常量求值
    }
    return x + 1;
}

// 教学对比：有一条失败路径不代表函数非法
constexpr int clamp_positive(int x) {
    if (x < 0) {
        return 0;
    }
    return x;
}

// ---------------------------------------------------------------------------
// §专家 — 工程含义与和 consteval / 诊断的关系
// ---------------------------------------------------------------------------
// 1) 「永不常量求值」的 constexpr 函数：对调用方仍是普通函数；只是多了一个
//    可能误导的标记。应用场景主要是 *无条件统一 API 表面*。
// 2) 若你 *希望* 强制编译期，用 consteval，而不是依赖「标了 constexpr」。
// 3) 常量求值失败时诊断发生在 *使用点*（static_assert / constexpr 变量），
//    而不是函数定义点（C++23 对这类函数）。
// 4) 与 P2448 的「非字面值签名」放宽是同一提案包，服务同一目标。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section01/constexpr_function_never_constant_evaluated_cpp23 ===\n";

    int v = 99;
    reset_via_runtime_helper(v);
    assert(v == 0);
    std::cout << "[intro] reset_via_runtime_helper ran at runtime, v=" << v << '\n';

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L
    std::cout << "[intro] __cpp_constexpr=" << __cpp_constexpr
              << " — constexpr function that can never be constant-evaluated "
                 "is well-formed (P2448)\n";
    // 若取消注释：在需要常量表达式的上下文调用会失败
    // constexpr int bad = (reset_via_runtime_helper(v), 1);
#else
    std::cout << "[fallback] older __cpp_constexpr; demo uses constant-friendly body\n";
#endif

    static_assert(maybe_square(12) == 144);
    static_assert(branchy(3) == 4);
    static_assert(clamp_positive(-5) == 0);
    assert(maybe_square(v + 5) == 25);  // v==0

    std::string msg = "constexpr";
    log_and_touch(msg);
    assert(msg == "constexpr!");
    std::cout << "[advanced] maybe_square/branchy still evaluate at compile time; "
                 "log_and_touch is runtime-oriented: "
              << msg << '\n';

    std::cout << "[expert] Prefer marking APIs constexpr for *optional* CTCE; "
                 "use consteval when compile-time is mandatory; "
                 "never-CTCE constexpr is for uniform decoration, not proof of CTCE\n";

    std::cout << "constexpr_function_never_constant_evaluated_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section01/constexpr_function_never_constant_evaluated_cpp23", run>;

}  // namespace
