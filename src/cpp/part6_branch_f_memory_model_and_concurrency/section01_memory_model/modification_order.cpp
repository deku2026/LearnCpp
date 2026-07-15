// LearnCpp topic
// Doc      : 第6部分-支线F · F1.4 修改顺序
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : modification_order
// Topic id : part6/f/section01/modification_order
//
// 要点: 每个原子对象有单一 modification order; 所有线程对此一致。
//       不同原子对象之间 relaxed 不建立总序。
// 参考: [intro.races] modification order

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F1 modification order ===\n";

    std::atomic<int> x{0};

    // 单线程对同一原子的修改顺序即程序顺序
    x.store(1, std::memory_order_relaxed);
    x.store(2, std::memory_order_relaxed);
    x.store(3, std::memory_order_relaxed);
    assert(x.load(std::memory_order_relaxed) == 3);

    // 多线程 RMW 进入同一 modification order
    x.store(0, std::memory_order_relaxed);
    constexpr int kThreads = 4;
    constexpr int kPer = 1000;
    std::vector<std::jthread> threads;
    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&] {
            for (int i = 0; i < kPer; ++i) {
                x.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    threads.clear();  // join all
    assert(x.load() == kThreads * kPer);
    std::cout << "  RMW total " << x.load() << " (single mod order, no lost updates)\n";

    // 读只能看到 modification order 中"某次写及其后"的值 (连贯性)
    // 教学断言: 单调写序列下读不会读到"中间回退"到未出现过的值
    std::atomic<int> mono{0};
    std::atomic<bool> stop{false};
    std::jthread writer([&] {
        for (int i = 1; i <= 100; ++i) {
            mono.store(i, std::memory_order_relaxed);
        }
        stop.store(true, std::memory_order_release);
    });
    std::jthread reader([&] {
        int prev = 0;
        while (!stop.load(std::memory_order_acquire)) {
            int v = mono.load(std::memory_order_relaxed);
            assert(v >= prev);  // 值域在 0..100, 且不会看到乱序回退到更小的"已提交"外值
            // 注意: 可能重复读同一值; 但不会读到 > 当前已 store 的值之外的东西
            if (v > prev) {
                prev = v;
            }
        }
        int last = mono.load(std::memory_order_relaxed);
        assert(last == 100);
    });
    writer = std::jthread{};
    reader = std::jthread{};

    std::cout << "  different atomics: no shared modification order under relaxed\n";
    std::cout << "modification_order: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/modification_order", run>;

}  // namespace
