// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section02_if_consteval_cpp23
// Item     : if_consteval_calls_consteval_function
// Topic id : part2/stage07/section02/if_consteval_calls_consteval_function
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/if
//   https://en.cppreference.com/w/cpp/language/consteval
//   P1938R3 — if consteval
//   验收点：为什么 if consteval 的 true 分支能调 consteval，而
//           if (is_constant_evaluated()) 不能

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — consteval 立即函数：每次（可求值）调用都必须是编译期常量
// ---------------------------------------------------------------------------

consteval int immediate_square(int n) {
    return n * n;
}

consteval int immediate_mul(int a, int b) {
    return a * b;
}

// ---------------------------------------------------------------------------
// §进阶 — if consteval 的 true 分支 = 立即函数上下文
// ---------------------------------------------------------------------------
// 在立即函数上下文中调用 consteval 函数时，不要求该调用表达式本身
// 在 *封闭函数的普通语义* 下已是常量表达式——因为 true 分支只会在
// 编译期执行，参数在该语境下按可常量求值处理。
//
// 对比：if (std::is_constant_evaluated()) 只是普通 if，两个分支都按
// 常规代码检查；在 true 分支写 immediate_square(val) 会因 val 不是
// 常量表达式而编译失败（即使逻辑上只在编译期走到）。

constexpr int good_with_if_consteval(int val) {
    if consteval {
        return immediate_square(val);  // OK：立即函数上下文
    } else {
        return val * val;  // 运行期普通乘法
    }
}

// 错误模式（保留为注释，取消会编译失败）：
// constexpr int bad_with_ice(int val) {
//     if (std::is_constant_evaluated()) {
//         return immediate_square(val); // error: val not a constant expression
//     }
//     return val * val;
// }

// 可编译的 ice 版本：true 分支 *不能* 调 consteval，只能用手写算法
constexpr int ice_without_consteval(int val) {
    if (std::is_constant_evaluated()) {
        return val * val;  // 无法复用 immediate_square
    }
    return val * val;
}

// ---------------------------------------------------------------------------
// §专家 — 双路径：编译期精确算法（可调 consteval 子程序）/ 运行期快路径
// ---------------------------------------------------------------------------

consteval double newton_sqrt_step(double x, double guess) {
    // 拆成 consteval 子步骤，模拟「仅编译期可用的内核」
    return (guess + x / guess) / 2.0;
}

consteval double newton_sqrt_immediate(double x) {
    if (x < 0.0) {
        return -1.0;
    }
    if (x == 0.0) {
        return 0.0;
    }
    double guess = x;
    double prev = 0.0;
    // 固定迭代上限，保证编译期终止
    for (int i = 0; i < 32; ++i) {
        prev = guess;
        guess = newton_sqrt_step(x, guess);
        if (guess == prev) {
            break;
        }
    }
    return guess;
}

constexpr double my_sqrt(double x) {
    if consteval {
        return newton_sqrt_immediate(x);  // 调 consteval 链
    } else {
        return std::sqrt(x);
    }
}

// 组合：编译期用 immediate 做范围检查，运行期用宽松策略
constexpr int checked_index(int i, int n) {
    if consteval {
        // 编译期：非法下标直接让常量求值失败更安全；这里用夹紧 + 标记
        if (i < 0 || i >= n) {
            return immediate_mul(-1, 1);  // -1
        }
        return i;
    } else {
        if (i < 0) {
            return 0;
        }
        if (i >= n) {
            return n - 1;
        }
        return i;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section02/if_consteval_calls_consteval_function ===\n";

    static_assert(immediate_square(5) == 25);
    static_assert(good_with_if_consteval(9) == 81);
    static_assert(ice_without_consteval(9) == 81);

    constexpr double root2 = my_sqrt(2.0);
    static_assert(root2 > 1.414 && root2 < 1.415);

    constexpr int idx = checked_index(2, 10);
    static_assert(idx == 2);
    static_assert(checked_index(-3, 10) == -1);  // 编译期哨兵

    std::cout << "[intro] immediate_square(5)=" << immediate_square(5) << '\n';
    std::cout << "[advanced] good_with_if_consteval(9) CT=" << good_with_if_consteval(9) << '\n';

    int v = 9;
    assert(good_with_if_consteval(v) == 81);
    assert(ice_without_consteval(v) == 81);
    std::cout << "[advanced] runtime good_with_if_consteval(9)=" << good_with_if_consteval(v)
              << " (else branch; no consteval call)\n";

    double x = 2.0;
    const double rt = my_sqrt(x);
    assert(rt > 1.41 && rt < 1.42);
    std::cout << "[expert] my_sqrt: CT=" << root2 << " RT=" << rt << " (CT used consteval newton kernel)\n";

    int i = 100;
    assert(checked_index(i, 10) == 9);  // 运行期夹紧
    std::cout << "[expert] checked_index runtime clamps 100→" << checked_index(i, 10)
              << "; compile-time returns -1 for OOB\n";

    std::cout << "[expert] KEY: if consteval true-branch is an immediate "
                 "function context → may call consteval; "
                 "if(is_constant_evaluated()) is ordinary → may not\n";

    std::cout << "if_consteval_calls_consteval_function: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section02/if_consteval_calls_consteval_function", run>;

}  // namespace
