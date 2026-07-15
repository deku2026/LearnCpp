// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B4 Meyers Singleton)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : meyers_singleton
// Topic id : part6/b/section01/meyers_singleton
//
// 要点: 函数局部 static 首次调用时初始化(C++11 线程安全)，修 SIOF。
//       Core Guidelines 更倾向依赖注入；Meyers 是「确需惰性全局」时的标准手法。
// 参考: [stmt.dcl] function-local static

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

struct Registry {
    std::string name;
    int touch_count = 0;
    explicit Registry(std::string n) : name(std::move(n)) { std::cout << "  Registry constructed: " << name << '\n'; }
    void touch() { ++touch_count; }
};

// Meyers Singleton：惰性 + 首次使用即初始化
Registry& get_registry() {
    static Registry r{"app"};
    return r;
}

// 进阶: 构造可能失败时，仍应保证「要么完全构造，要么抛出且可重试」
// （标准：若首次初始化抛异常，下次进入会再次尝试初始化）

// 专家: 与「饿汉」全局对比
// Registry g_eager{"eager"};  // 动态初始化，跨 TU 可能 SIOF
// Meyers 把初始化推迟到首次调用，切断跨 TU 启动顺序依赖。

std::atomic<int> g_construct_log{0};

struct Counted {
    Counted() { g_construct_log.fetch_add(1, std::memory_order_relaxed); }
    int v = 1;
};

Counted& counted_singleton() {
    static Counted c;
    return c;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B4 Meyers singleton (function-local static) ===\n";

    // --- 入门: 首次调用才构造；同一实例 ---
    Registry& a = get_registry();
    assert(a.name == "app");
    a.touch();

    Registry& b = get_registry();
    assert(&a == &b);
    assert(b.touch_count == 1);
    b.touch();
    assert(a.touch_count == 2);

    // --- 进阶: C++11 起初始化线程安全（多线程首次进入只构造一次）---
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([] {
            counted_singleton().v += 0;  // 触发初始化
            get_registry().touch();
        });
    }
    for (auto& t : threads) t.join();

    assert(g_construct_log.load() == 1);  // Counted 只构造一次
    // 主线程 touch 2 次 + 8 线程各 1 次 = 10
    assert(get_registry().touch_count == 10);

    // --- 专家笔记 ---
    std::cout << "  C++11 magic static objects: concurrent first-entry is serialized\n";
    std::cout << "  note: Core Guidelines prefer DI over global singleton;\n";
    std::cout << "        local static is the standard SIOF fix when needed.\n";
    std::cout << "  destruction: still at program exit (reverse of completion);\n";
    std::cout << "  avoid depending on other globals inside static dtors.\n";
    std::cout << "meyers_singleton: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/meyers_singleton", run>;

}  // namespace
