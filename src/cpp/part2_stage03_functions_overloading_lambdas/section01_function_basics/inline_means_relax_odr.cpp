// Topic     : inline 的真实含义：放宽 ODR，不是强制内联
// Doc       : 第2部分-阶段3 · 步骤 2
// cppreference: https://en.cppreference.com/cpp/language/inline
//               https://en.cppreference.com/cpp/language/definition
//
// 要点: inline 允许多 TU 各有一份相同定义并由链接器合并；
//       类内成员 / constexpr / 函数模板 隐式 inline；
//       「内联展开」由优化器决定，与关键字基本无关。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>

// 模拟「放在头文件里、被多个 .cpp include」的工具函数
inline int add(int a, int b) {
    return a + b;
}

// C++17 inline 变量：同样可在头里定义全局/命名空间作用域变量
inline int g_header_counter = 0;

struct Point {
    int x = 0;
    int y = 0;
    // 类内定义的成员函数 → 隐式 inline
    int sum() const { return x + y; }
};

// constexpr 函数 → 隐式 inline
constexpr int square(int n) {
    return n * n;
}

// 函数模板 → 隐式 inline（定义本就常在头文件）
template <class T>
T max2(T a, T b) {
    return a > b ? a : b;
}

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [inline_means_relax_odr] ===\n";

    // -------------------------------------------------------------------------
    // §入门：inline 让「头文件里的定义」合法
    // -------------------------------------------------------------------------
    assert(add(2, 3) == 5);
    ++g_header_counter;
    assert(g_header_counter >= 1);
    std::cout << "[intro] inline function/variable usable as if from a header; "
              << "counter=" << g_header_counter << '\n';

    // -------------------------------------------------------------------------
    // §进阶：天生 inline 的实体
    // -------------------------------------------------------------------------
    [[maybe_unused]] Point p{3, 4};
    assert(p.sum() == 7);
    static_assert(square(5) == 25);
    assert(max2(10, 20) == 20);
    // 这些不写 inline 关键字也能安全放进头文件——因为语言已赋予 inline 语义
    std::cout << "[advanced] class-inline member / constexpr / template are implicitly inline\n";

    // -------------------------------------------------------------------------
    // §专家：ODR 与「强制内联」的误解
    // -------------------------------------------------------------------------
    // 1) inline 保证：每个用到它的 TU 可以有定义；定义必须相同；链接器合并成单一实体。
    // 2) 不写 inline 的非模板函数若在多个 TU 各定义一次 → multiple definition 链接错误。
    // 3) 「内联展开」：-O2 下即使无 inline 关键字也可能展开；写了 inline 也可能不展开。
    //    为性能到处加 inline 是无效习惯；正当用途是放宽 ODR。
    // 4) inline 变量（C++17）解决「头文件全局变量」的同一问题。
    // 5) 与 inline namespace（版本化）是不同机制，勿混淆。
    static_assert(std::is_same_v<decltype(add(1, 2)), int>);
    assert(square(0) == 0 && max2(-1, -2) == -1);

    std::cout << "[expert] inline = ODR relaxation; optimizer alone decides inlining\n";
    std::cout << "=== inline_means_relax_odr: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/inline_means_relax_odr", run>;

}  // namespace
