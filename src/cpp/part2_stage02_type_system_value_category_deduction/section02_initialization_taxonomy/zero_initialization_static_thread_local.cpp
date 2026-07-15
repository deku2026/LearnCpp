// Topic     : 静态 / thread_local 的零初始化
// Doc       : 第2部分-阶段2 · 步骤 2.5
// cppreference: https://en.cppreference.com/cpp/language/zero_initialization
//               https://en.cppreference.com/cpp/language/storage_duration
//               https://en.cppreference.com/cpp/language/initialization
//
// 要点: 静态存储期与线程存储期对象在其它初始化之前先零初始化；
//       自动存储期局部变量没有这个保证。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {
[[maybe_unused]]

int g_global;                            // 静态存储期 → 零初始化 →[[maybe_unused]]  0
[[maybe_unused]] static int g_internal;  // 内部链接，同样先零初始化

struct Pod {
    int a;
    int* p;
};
[[maybe_unused]]

Pod g_pod;  // 成员全零 / 空指针

thread_local int tls_counter;  // 每个线程一份，首次也先零初始化

void bump_static_local() {
    static int calls;  // 首次进入前零初始化
    ++calls;
    assert(calls >= 1);
    std::cout << "[intro] static local calls=" << calls << '\n';
}

int run(int /*argc*/, char** /*argv*/) {
    (void)g_global;
    (void)g_internal;
    (void)g_pod;

    std::cout << "=== [zero_initialization_static_thread_local] ===\n";

    // -------------------------------------------------------------------------
    // 入门：全局与 static 局部
    // -------------------------------------------------------------------------
    assert(g_global == 0);
    assert(g_internal == 0);
    assert(g_pod.a == 0 && g_pod.p == nullptr);

    bump_static_local();
    bump_static_local();  // 第二次保持状态

    // 对照：自动变量不会零初始化
    // int auto_var; // 不确定
    [[maybe_unused]] int auto_var{};
    assert(auto_var == 0);
    std::cout << "[intro] globals and static-duration objects zeroed before dynamic init\n";

    // -------------------------------------------------------------------------
    // 进阶：thread_local
    // -------------------------------------------------------------------------
    assert(tls_counter == 0);
    ++tls_counter;
    assert(tls_counter == 1);
    // 其它线程看到的是各自的 0 起步副本（此处单线程演示）
    std::cout << "[advanced] thread_local tls_counter=" << tls_counter << '\n';

    // 带动态初始化的静态：先零，再跑初始化器
    static std::string s = "hello";  // 动态初始化
    assert(s == "hello");
    [[maybe_unused]] static int from_zero_then_set = 5;  // 先 0 再变成 5（实现细节级理解）
    assert(from_zero_then_set == 5);

    // -------------------------------------------------------------------------
    // 专家：初始化顺序与常量初始化
    // -------------------------------------------------------------------------
    // 零初始化 ⊂ 静态初始化；常量初始化可在编译期完成。
    // 跨翻译单元的非局部静态初始化顺序未指定（static init order fiasco）——
    // 用函数内 static 局部 / 避免依赖顺序。
    // constinit（C++20）要求静态初始化成功，防止动态初始化惊喜。

    static int* dangling_pattern = nullptr;  // 明确零
    assert(dangling_pattern == nullptr);
    (void)dangling_pattern;

    std::cout << "[expert] zero-init is not a substitute for explicit init of locals\n";
    std::cout << "=== zero_initialization_static_thread_local: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/zero_initialization_static_thread_local", run>;

}  // namespace
