// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B4 SIOF / B12 验收)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : static_initialization_order_problem
// Topic id : part6/b/section01/static_initialization_order_problem
//
// 要点: 跨 TU 非局部静态对象初始化顺序未规定(SIOF)；
//       同 TU 按定义顺序安全。修法: constinit / 函数局部 static。
// 验收: 解释 SIOF 并用 constinit/函数局部静态修好。
// 参考: https://isocpp.org/wiki/faq/ctors#static-init-order  [basic.start.static]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// --- 入门: 同 TU 内顺序确定，依赖安全 ---
struct Config {
    int value;
    explicit Config(int v) : value(v) { std::cout << "  Config(" << value << ") ctor\n"; }
    int get() const { return value; }
};

// 同文件：g_config 先定义，g_cached 后定义 → 安全
Config g_config{42};
int g_cached = g_config.get();  // 同 TU 顺序确定

// 跨 TU 危险模式（注释，勿拆成多文件真跑 UB）:
// // a.cpp: int g = g_config.get();   // g_config 在 b.cpp
// // b.cpp: Config g_config{42};
// // 若 a 的动态初始化先于 b → 读未构造对象 → UB

// --- 进阶: 函数局部 static 切断跨 TU 依赖 ---
const Config& safe_config() {
    static Config cfg{100};  // 首次调用时构造；C++11 起线程安全
    return cfg;
}

// constinit 路径：编译期就绪，无动态 init 顺序
constinit int g_flag = 1;

// --- 专家: destruction order fiasco 提示 ---
// 程序结束时静态对象析构顺序 = 构造完成顺序的逆序（同 TU 内确定；
// 跨 TU 仍可能「先析构被依赖者」）。长寿命依赖短寿命全局 → 析构期 UB。
// 防御: 不在析构中依赖其他全局；或故意 leak 进程级资源。

struct DependsOnConfig {
    int snapshot;
    explicit DependsOnConfig() : snapshot(safe_config().get()) {
        std::cout << "  DependsOnConfig snapshot=" << snapshot << "\n";
    }
};

DependsOnConfig& depends() {
    static DependsOnConfig d;  // 首次用时构造；保证 safe_config 已就绪
    return d;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B4 SIOF (static init order fiasco) ===\n";

    // 同 TU：确定且安全
    assert(g_config.get() == 42);
    assert(g_cached == 42);
    std::cout << "  same-TU static order: g_cached=" << g_cached << '\n';

    // 惰性：无论谁先调用，首次访问时才构造
    assert(safe_config().get() == 100);
    assert(&safe_config() == &safe_config());  // 同一实例

    assert(g_flag == 1);
    g_flag = 2;
    assert(g_flag == 2);

    assert(depends().snapshot == 100);

    std::cout << "  SIOF root: cross-TU non-local static *dynamic* order unspecified\n";
    std::cout << "  remedies:\n";
    std::cout << "    1) constinit / constexpr constant init (no dynamic phase)\n";
    std::cout << "    2) function-local static (Meyers) — init on first use\n";
    std::cout << "  also watch destruction-order fiasco at shutdown\n";
    std::cout << "static_initialization_order_problem: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/static_initialization_order_problem", run>;

}  // namespace
