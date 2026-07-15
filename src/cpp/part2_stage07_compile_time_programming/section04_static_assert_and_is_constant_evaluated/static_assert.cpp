// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section04_static_assert_and_is_constant_evaluated
// Item     : static_assert
// Topic id : part2/stage07/section04/static_assert
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/static_assert
//   ISO [dcl.pre] / static_assert declaration
//   C++11: static_assert(expr, message)
//   C++17: message optional
//   C++26: 更丰富的消息形态（本文件以 C++23 为基准）

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — static_assert：编译期断言，失败则编译失败
// ---------------------------------------------------------------------------
// 条件必须是上下文相关的常量表达式，可转换为 bool。
// 这是验证「constexpr 真的算对了」的标准手段，也是路线图验收点。

constexpr int factorial(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

static_assert(factorial(0) == 1);
static_assert(factorial(5) == 120, "5! must be 120");
// static_assert(factorial(5) == 100, "wrong"); // 取消注释 → 编译失败

// ---------------------------------------------------------------------------
// §进阶 — 类型特性、模板约束前史、作用域
// ---------------------------------------------------------------------------

template <class T, std::size_t N>
struct SimpleArray {
    T data[N];
    static_assert(N > 0, "SimpleArray size must be positive");
    static_assert(std::is_trivially_copyable_v<T> || std::is_class_v<T>, "demo: T should be usable as value element");
};

template <class T>
constexpr T add_same(T a, T b) {
    static_assert(std::is_arithmetic_v<T>, "add_same expects arithmetic T");
    return a + b;
}

// C++17: 可省略消息
static_assert(sizeof(void*) >= 4);

// 命名空间 / 块 / 类作用域均可
struct Widget {
    static constexpr int kVersion = 3;
    static_assert(kVersion >= 1, "Widget version");
};

// ---------------------------------------------------------------------------
// §专家 — 与 assert、concept、if constexpr、编译期 UB 的关系
// ---------------------------------------------------------------------------
// 1) assert：运行期（NDEBUG 可剥离）；static_assert：编译期，不可剥离。
// 2) 概念/requires 表达「约束」；static_assert 表达「已知应成立的事实」。
// 3) 条件里的 UB（越界、有符号溢出）在常量求值中 → 编译失败（安全收益）。
// 4) 消息字符串字面量：C++23 仍是字符串字面量；更动态的消息属后续标准方向。
// 5) static_assert(false) 在模板中若未被实例化可能不触发——与
//    if constexpr 丢弃分支结合时要小心（依赖实现/标准细化）。

template <class T>
constexpr int bit_width_hint() {
    if constexpr (sizeof(T) == 4) {
        return 32;
    } else if constexpr (sizeof(T) == 8) {
        return 64;
    } else {
        // 对不支持的尺寸：在实例化路径上 static_assert
        static_assert(sizeof(T) == 4 || sizeof(T) == 8, "only 4 or 8 byte T supported in this demo");
        return 0;
    }
}

constexpr int safe_index(std::array<int, 3> a, std::size_t i) {
    // 编译期调用时越界 → 常量求值失败；这里先断言再访问
    if (i >= a.size()) {
        return -1;
    }
    return a[i];
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section04/static_assert ===\n";

    static_assert(add_same(2, 3) == 5);
    static_assert(bit_width_hint<int>() == 32 || bit_width_hint<int>() == 64);
    static_assert(safe_index(std::array<int, 3>{10, 20, 30}, 1) == 20);

    [[maybe_unused]] SimpleArray<int, 4> sa{{1, 2, 3, 4}};
    assert(sa.data[3] == 4);

    std::cout << "[intro] factorial(5)=" << factorial(5) << " verified by static_assert at compile time\n";
    std::cout << "[advanced] add_same(2,3)=" << add_same(2, 3) << " Widget::kVersion=" << Widget::kVersion << '\n';

    // 运行期 assert 对照
    assert(factorial(4) == 24);
    std::cout << "[expert] assert checks at runtime; static_assert already "
                 "validated factorial(5)==120 before main started\n";

    // 展示失败信息风格（不真正失败）
    std::cout << "[expert] if static_assert fails, compilation stops with the "
                 "message string — earlier than any test runner\n";

    std::cout << "static_assert: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section04/static_assert", run>;

}  // namespace
