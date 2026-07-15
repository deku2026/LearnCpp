// LearnCpp topic
// Doc      : 第6部分-支线F · F10 volatile 与线程
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : volatile_versus_thread_synchronization
// Topic id : part6/f/section05/volatile_versus_thread_synchronization
//
// 要点: volatile ≠ 原子/互斥; 只抑制编译器对"内存映射/信号"场景的优化。
//       线程同步请用 std::atomic / mutex。
// 参考: [dcl.type.cv] Sutter "volatile vs volatile"

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F10 volatile vs thread synchronization ===\n";

    // 正确: atomic
    std::atomic<bool> go{false};
    int data = 0;
    std::jthread t([&] {
        while (!go.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        assert(data == 42);
    });
    data = 42;
    go.store(true, std::memory_order_release);
    t = std::jthread{};

    // volatile 的合法用途示意: 模拟 MMIO 寄存器 (单线程可见性对编译器)
    volatile int device_reg = 0;
    device_reg = 1;
    device_reg = 2;
    assert(device_reg == 2);
    // 每次访问都生成真实 load/store (相对非 volatile), 但仍无跨线程 happens-before

    std::cout << "  volatile: compiler must not elide/reorder vs other volatiles\n";
    std::cout << "  NOT: atomicity, mutual exclusion, acquire/release\n";
    std::cout << "  Java/C# volatile ≈ 更接近原子可见性 — 与 C++ 不同!\n";

    // 错误模式 (不运行): volatile bool flag; thread writes data+flag; other reads
    // 在 C++ 中仍是数据竞争 UB。

    std::cout << "volatile_versus_thread_synchronization: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/volatile_versus_thread_synchronization", run>;

}  // namespace
