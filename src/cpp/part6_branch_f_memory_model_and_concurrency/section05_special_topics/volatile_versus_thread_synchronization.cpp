// LearnCpp topic
// Doc      : 第6部分-支线F · F10 volatile 与线程
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : volatile_versus_thread_synchronization
// Topic id : part6/f/section05/volatile_versus_thread_synchronization
//
// 要点: volatile ≠ 原子/互斥; 只抑制编译器对 MMIO/信号场景的优化。
// 线程同步请用 std::atomic / mutex。不演示 data race。
// 参考: [dcl.type.cv] Sutter "volatile vs volatile"

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F10 volatile vs thread synchronization ===\n";

    // 正确: atomic 同步
    std::atomic<bool> go{false};
    [[maybe_unused]] int data = 0;
    std::jthread t([&] {
        while (!go.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        assert(data == 42);
    });
    data = 42;
    go.store(true, std::memory_order_release);
    t = std::jthread{};

    // volatile 合法: 模拟 MMIO（对编译器强制真实访问）
    [[maybe_unused]] volatile int device_reg = 0;
    device_reg = 1;
    device_reg = 2;
    assert(device_reg == 2);
    // 仍无跨线程 happens-before

    std::cout << "  volatile: must not elide/reorder vs other volatiles (compiler)\n";
    std::cout << "  NOT: atomicity, mutual exclusion, acquire/release\n";
    std::cout << "  Java/C# volatile ≈ atomic visibility — NOT the same as C++\n";
    std::cout << "  wrong: volatile bool flag for thread handoff → still data race\n";
    std::cout << "volatile_versus_thread_synchronization: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/volatile_versus_thread_synchronization", run>;

}  // namespace
