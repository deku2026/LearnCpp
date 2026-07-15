// LearnCpp topic
// Doc      : 第6部分-支线G · 未初始化读
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : uninitialized_read_deep
// Topic id : part6/g/section03/uninitialized_read_deep
//
// 要点: 读未初始化自动存储期对象通常是 UB; 默认初始化/值初始化分清。
// 不触发 UB。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>
#include <vector>

namespace {

struct Pod {
    int a;
    int b;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G uninitialized read (safe patterns) ===\n";

    // 值初始化: 零
    [[maybe_unused]] int x{};
    assert(x == 0);
    [[maybe_unused]] Pod p{};
    assert(p.a == 0 && p.b == 0);

    // 默认初始化内置类型在块作用域不初始化 —— 不要读
    // int y; use(y); // UB — 不写

    [[maybe_unused]] int y = 0;  // 明确初始化
    assert(y == 0);

    std::vector<int> v(3);  // 值初始化元素为 0
    assert(v[0] == 0 && v[2] == 0);

    std::optional<int> o;
    assert(!o.has_value());
    o = 5;
    assert(*o == 5);

    // 成员: 用 NSDMI / 构造函数初始化列表
    struct S {
        int n = 1;
    };
    assert(S{}.n == 1);

    std::cout << "  MSan finds uninit on supported platforms\n";
    std::cout << "  {} value-init; =0 explicit; optional for 'maybe'\n";
    std::cout << "uninitialized_read_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/uninitialized_read_deep", run>;

}  // namespace
