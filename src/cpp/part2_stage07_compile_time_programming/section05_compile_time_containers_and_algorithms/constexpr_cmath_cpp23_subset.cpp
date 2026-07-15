// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_cmath_cpp23_subset
// Topic id : part2/stage07/section05/constexpr_cmath_cpp23_subset
//
// Refs:
//   https://en.cppreference.com/w/cpp/numeric/math
//   P0533R9 — constexpr for <cmath> / <cstdlib>
//   Feature-test: __cpp_lib_constexpr_cmath
//   文档说明：部分实现上 std::sqrt/pow 仍不能直接 static_assert，
//             教学上用 if consteval 手写编译期路径 + 标准库运行期路径。

#include "learn/topic_registry.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <version>

namespace {

// ---------------------------------------------------------------------------
// §入门 — C++23 让一批 <cmath>/<cstdlib> 函数变为 constexpr
// ---------------------------------------------------------------------------
// 历史债：std::chrono::abs 早就可 constexpr，而 std::abs 长期不行。
// P0533 补齐：abs、fabs、fmod、有限性分类等（实现覆盖度不一）。

#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202202L

constexpr int abs_demo(int x) {
    return std::abs(x);
}

constexpr long labs_demo(long x) {
    return std::labs(x);
}

constexpr bool finite_demo(double x) {
    return std::isfinite(x);
}

#else

constexpr int abs_demo(int x) {
    return x < 0 ? -x : x;
}

constexpr long labs_demo(long x) {
    return x < 0 ? -x : x;
}

constexpr bool finite_demo(double x) {
    return x == x && x != std::numeric_limits<double>::infinity() && x != -std::numeric_limits<double>::infinity();
}

#endif

// ---------------------------------------------------------------------------
// §进阶 — 编译期数学：手写 constexpr 内核 + 运行期 std 快路径
// ---------------------------------------------------------------------------

constexpr double constexpr_fabs(double x) {
    return x < 0 ? -x : x;
}

constexpr double constexpr_sqrt(double x) {
    if (x < 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (x == 0) {
        return 0;
    }
    double g = x;
    for (int i = 0; i < 40; ++i) {
        g = 0.5 * (g + x / g);
    }
    return g;
}

constexpr double smart_sqrt(double x) {
    if consteval {
        return constexpr_sqrt(x);
    } else {
        return std::sqrt(x);
    }
}

constexpr double smart_fabs(double x) {
    if consteval {
        return constexpr_fabs(x);
    } else {
        return std::fabs(x);
    }
}

// ---------------------------------------------------------------------------
// §专家 — 实现现状与可移植策略
// ---------------------------------------------------------------------------
// 1) 查 __cpp_lib_constexpr_cmath 再决定是否 static_assert(std::abs(...))。
// 2) 浮点常量求值的舍入在实现间可能有细微差别 → 用范围断言。
// 3) 特殊值（NaN/Inf）在 constexpr 中的行为需对照 [cmath.syn] / 实现文档。
// 4) 库代码常见模式：if consteval 手写 + else 调 std。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/constexpr_cmath_cpp23_subset ===\n";

#if defined(__cpp_lib_constexpr_cmath)
    std::cout << "[intro] __cpp_lib_constexpr_cmath=" << __cpp_lib_constexpr_cmath << '\n';
#else
    std::cout << "[intro] __cpp_lib_constexpr_cmath not defined; using portable "
                 "fallbacks for abs/isfinite demos\n";
#endif

    static_assert(abs_demo(-12) == 12);
    static_assert(labs_demo(-100L) == 100L);
    static_assert(finite_demo(1.25));

    constexpr double r2 = smart_sqrt(2.0);
    static_assert(r2 > 1.414 && r2 < 1.415);
    static_assert(smart_fabs(-3.5) == 3.5);

    std::cout << "[intro] abs_demo(-12)=" << abs_demo(-12) << '\n';
    std::cout << "[advanced] smart_sqrt(2) CT=" << r2 << '\n';

    double x = 2.0;
    const double rt = smart_sqrt(x);
    assert(rt > 1.41 && rt < 1.42);
    assert(smart_fabs(-x) == 2.0);

    std::cout << "[advanced] smart_sqrt runtime=" << rt << " smart_fabs(-2)=" << smart_fabs(-x) << '\n';
    std::cout << "[expert] Prefer if consteval dual-path for portable constexpr "
                 "math until your STL fully constexpr-izes <cmath>\n";

    // 若库支持，额外用 std::abs 在常量上下文试探
#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202202L
    static_assert(std::abs(-5) == 5);
    std::cout << "[expert] std::abs is usable in CTCE on this STL\n";
#endif

    std::cout << "constexpr_cmath_cpp23_subset: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_cmath_cpp23_subset", run>;

}  // namespace
