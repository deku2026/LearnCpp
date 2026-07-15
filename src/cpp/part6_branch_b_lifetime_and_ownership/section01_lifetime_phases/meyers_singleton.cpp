// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B4 Meyers Singleton)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : meyers_singleton
// Topic id : part6/b/section01/meyers_singleton
//
// 要点: 函数局部 static 首次调用时初始化(C++11 线程安全)，规避 SIOF。
// 参考: [stmt.dcl] function-local static

#include "learn/topic_registry.hpp"

#include <cassert>
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

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B4 Meyers singleton (function-local static) ===\n";

    // 首次调用才构造
    Registry& a = get_registry();
    assert(a.name == "app");
    a.touch();

    Registry& b = get_registry();
    assert(&a == &b);  // 同一实例
    assert(b.touch_count == 1);
    b.touch();
    assert(a.touch_count == 2);

    // C++11 起初始化线程安全：多线程首次进入只构造一次
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([] { get_registry().touch(); });
    }
    for (auto& t : threads) t.join();
    assert(get_registry().touch_count == 6);

    std::cout << "  note: Core Guidelines prefer DI over global singleton;\n";
    std::cout << "        local static is the standard SIOF fix when needed.\n";
    std::cout << "meyers_singleton: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/meyers_singleton", run>;

}  // namespace
