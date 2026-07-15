// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B4 constinit)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : constinit_remedy
// Topic id : part6/b/section01/constinit_remedy
//
// 要点: constinit 强制常量初始化 → 无运行期动态初始化顺序问题；
//       变量本身仍可变（区别于 constexpr）。
// 参考: https://en.cppreference.com/w/cpp/language/constinit

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// 编译期完成初始化 → 无 SIOF
constinit int g_value = 42;
constinit int g_table[] = {1, 2, 3, 4};

// constexpr 变量也是常量初始化，但对象本身 const
constexpr int k_limit = 100;

// 非法（示意，勿取消注释）：
// int runtime_seed();
// constinit int bad = runtime_seed();  // 非常量表达式初始化器

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B4 constinit remedy for SIOF ===\n";

    assert(g_value == 42);
    g_value = 7;  // constinit 对象可变
    assert(g_value == 7);

    assert(g_table[0] == 1 && g_table[3] == 4);
    g_table[0] = 99;
    assert(g_table[0] == 99);

    // constexpr 是常量对象
    static_assert(k_limit == 100);
    // k_limit = 1;  // 非法

    // constinit 要求初始化器是常量表达式；适合无依赖运行时的全局
    constinit static int local_static = 5;
    assert(local_static == 5);
    local_static = 6;
    assert(local_static == 6);

    std::cout << "  constinit: compile-time init, runtime mutable\n";
    std::cout << "  if init needs runtime → use Meyers singleton instead\n";
    std::cout << "constinit_remedy: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/constinit_remedy", run>;

}  // namespace
