// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B4 constinit / B12 验收)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : constinit_remedy
// Topic id : part6/b/section01/constinit_remedy
//
// 要点: constinit 强制常量初始化 → 绕开跨 TU 动态初始化顺序问题(SIOF)；
//       变量本身仍可运行期修改（不同于 constexpr 对象）。
// 验收: 解释 SIOF + 用 constinit / 函数局部 static 修好。
// 参考: https://en.cppreference.com/w/cpp/language/constinit  [dcl.constinit]

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

namespace {

// --- 入门: 常量初始化的全局，无动态初始化顺序依赖 ---
constinit int g_value = 42;
constinit int g_table[] = {1, 2, 3, 4};

// constexpr 对象也是常量初始化，但绑定 const（不可改）
constexpr int k_limit = 100;

// 进阶: constinit + 编译期计算
consteval int square(int x) {
    return x * x;
}
constinit int g_sq = square(9);  // 81，编译期完成

// 进阶: 聚合 / 数组仍可 constinit
struct Point {
    int x;
    int y;
};
constinit Point g_origin{0, 0};
constinit std::array<int, 4> g_arr{10, 20, 30, 40};

// 非法示例（勿解注）:
// int runtime_seed();
// constinit int bad = runtime_seed();  // 非常量表达式初始化器 → ill-formed

// 专家: constinit 解决「动态初始化」顺序；若初始化必须运行期，改用 Meyers
int& lazy_counter() {
    static int n = 0;  // 首次调用才动态初始化，跨 TU 安全
    return n;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B4 constinit remedy for SIOF ===\n";

    // --- 入门 ---
    assert(g_value == 42);
    g_value = 7;  // constinit 对象可变
    assert(g_value == 7);

    assert(g_table[0] == 1 && g_table[3] == 4);
    g_table[0] = 99;
    assert(g_table[0] == 99);

    static_assert(k_limit == 100);
    // k_limit = 1;  // ill-formed: constexpr 对象是 const

    // --- 进阶: 编译期算好的全局 ---
    assert(g_sq == 81);
    g_origin.x = 1;
    assert(g_origin.x == 1 && g_origin.y == 0);
    assert(g_arr[2] == 30);
    g_arr[2] = 33;
    assert(g_arr[2] == 33);

    // 函数内 constinit static：仍要求常量初始化器
    constinit static int local_static = 5;
    assert(local_static == 5);
    local_static = 6;
    assert(local_static == 6);

    // --- 专家对照 ---
    // constinit: 编译期完成 → 无 SIOF，适合表、标志、POD 配置
    // Meyers:    首次使用时构造 → 可运行期依赖，C++11 线程安全
    lazy_counter() = 1;
    assert(lazy_counter() == 1);
    ++lazy_counter();
    assert(lazy_counter() == 2);

    std::cout << "  constinit: compile-time init, runtime mutable\n";
    std::cout << "  constexpr object: compile-time init, immutable\n";
    std::cout << "  if init needs runtime work → Meyers singleton instead\n";
    std::cout << "  SIOF root remains: cross-TU *dynamic* init order unspecified\n";
    std::cout << "constinit_remedy: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/constinit_remedy", run>;

}  // namespace
