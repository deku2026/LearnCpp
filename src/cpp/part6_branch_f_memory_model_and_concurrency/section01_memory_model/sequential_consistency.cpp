// LearnCpp topic
// Doc      : 第6部分-支线F · F2.2 seq_cst 总序直觉
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : sequential_consistency
// Topic id : part6/f/section01/sequential_consistency
//
// 要点: 顺序一致性 ≈ 所有线程看到同一全局交错; seq_cst 提供全序。
// 参考: Lamport; [atomics.order]

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F1/F2 sequential consistency (intuition) ===\n";

    // 教学: 两把旗 + seq_cst 不会出现 "双方都看到对方为 0" 的经典 store-buffer 重排
    // (在弱模型+非 seq_cst 下可能). 这里跑多次统计。
    int both_zero = 0;
    constexpr int kIters = 2000;

    for (int i = 0; i < kIters; ++i) {
        std::atomic<int> x{0};
        std::atomic<int> y{0};
        int r1 = 0, r2 = 0;

        std::jthread t1([&] {
            x.store(1, std::memory_order_seq_cst);
            r1 = y.load(std::memory_order_seq_cst);
        });
        std::jthread t2([&] {
            y.store(1, std::memory_order_seq_cst);
            r2 = x.load(std::memory_order_seq_cst);
        });
        t1 = std::jthread{};
        t2 = std::jthread{};

        if (r1 == 0 && r2 == 0) {
            ++both_zero;
        }
    }

    // 在正确 seq_cst 实现下 both_zero 应为 0
    assert(both_zero == 0);
    std::cout << "  Dekker-style loop: both_zero=" << both_zero << " over " << kIters << " iters (expect 0)\n";

    // 单线程视角: 操作像简单交错
    std::atomic<int> a{0};
    a.store(1);
    a.store(2);
    assert(a.load() == 2);

    std::cout << "  seq_cst = acquire/release + multi-copy atomicity / total order\n";
    std::cout << "sequential_consistency: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/sequential_consistency", run>;

}  // namespace
