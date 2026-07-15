// LearnCpp topic
// Doc      : 第6部分-支线F · F8.2 fence
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : memory_barriers
// Topic id : part6/f/section04/memory_barriers
//
// 要点: atomic_thread_fence 在原子操作之间插入屏障;
//       可与 relaxed 原子组合模拟 acq/rel (高级技巧)。
// 参考: [atomics.fences] cppreference atomic_thread_fence

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F4 / F8 memory barriers (fences) ===\n";

    // 用 fence 做发布
    int data = 0;
    std::atomic<bool> flag{false};

    std::jthread prod([&] {
        data = 77;
        std::atomic_thread_fence(std::memory_order_release);
        flag.store(true, std::memory_order_relaxed);
    });
    std::jthread cons([&] {
        while (!flag.load(std::memory_order_relaxed)) {
            std::this_thread::yield();
        }
        std::atomic_thread_fence(std::memory_order_acquire);
        assert(data == 77);
    });
    prod = std::jthread{};
    cons = std::jthread{};

    // seq_cst fence 更强 (全序)
    std::atomic_thread_fence(std::memory_order_seq_cst);

    // signal_fence: 仅限制编译器重排, 不发 CPU 屏障 (单线程/信号处理器场景)
    int x = 0;
    std::atomic_signal_fence(std::memory_order_acq_rel);
    x = 1;
    std::atomic_signal_fence(std::memory_order_acq_rel);
    assert(x == 1);

    std::cout << "  prefer ordered atomic ops; fences for advanced patterns\n";
    std::cout << "  thread_fence = compiler+CPU; signal_fence = compiler only\n";
    std::cout << "memory_barriers: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/memory_barriers", run>;

}  // namespace
