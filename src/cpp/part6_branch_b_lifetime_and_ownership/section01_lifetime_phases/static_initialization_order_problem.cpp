// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B4 SIOF)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : static_initialization_order_problem
// Topic id : part6/b/section01/static_initialization_order_problem
//
// 要点: 跨 TU 非局部静态对象初始化顺序未定义(SIOF)。
//       本文件用「同 TU 内安全」对照 + 说明跨 TU 风险；不制造 UB。
// 参考: https://isocpp.org/wiki/faq/ctors#static-init-order

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 同 TU：按定义顺序构造 —— 安全
struct Config {
    int value;
    explicit Config(int v) : value(v) {}
    int get() const { return value; }
};

// 同文件内：g_config 先定义，g_cached 后定义 → 安全
Config g_config{42};
int g_cached = g_config.get();  // 同 TU 顺序确定

// 跨 TU 的危险模式（注释，勿照抄到多文件）：
// // a.cpp: int g = g_config.get();  // g_config 在 b.cpp
// // b.cpp: Config g_config{42};
// // 若 a 的动态初始化先于 b → 读未构造对象 → UB

// 演示：用函数局部 static 规避跨 TU 依赖（见 meyers_singleton）
const Config& safe_config() {
    static Config cfg{100};
    return cfg;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B4 SIOF (static init order fiasco) ===\n";

    // 同 TU：确定、安全
    assert(g_config.get() == 42);
    assert(g_cached == 42);
    std::cout << "  same-TU static order: g_cached=" << g_cached << '\n';

    // 惰性初始化：无论谁先调用，首次访问时才构造
    assert(safe_config().get() == 100);
    assert(safe_config().get() == 100);  // 同一实例

    std::cout << "  SIOF root: cross-TU non-local static order is unspecified\n";
    std::cout << "  remedies: constinit (compile-time) / function-local static\n";
    std::cout << "static_initialization_order_problem: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/static_initialization_order_problem", run>;

}  // namespace
