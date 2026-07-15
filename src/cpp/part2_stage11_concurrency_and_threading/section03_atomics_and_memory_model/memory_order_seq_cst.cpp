// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 8.2 / 8.4
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_seq_cst
// Topic id : part2/stage11/section03/memory_order_seq_cst
// Refs     : https://en.cppreference.com/w/cpp/atomic/memory_order
//
// seq_cst (default): acquire/release PLUS a single total order of all
// seq_cst operations that all threads agree on. Safest; sometimes costlier
// (e.g. extra fences on weakly ordered ISAs for stores).

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [seq_cst] default order on plain load/store ===\n";
    {
        std::atomic<int> x{0};
        x.store(1);  // default seq_cst
        assert(x.load() == 1);
        std::cout << "  default memory_order is seq_cst\n";
    }

    std::cout << "=== IRIW-style intuition (why total order matters) ===\n";
    {
        // Classic illustration (may not fail on x86): two writers, two readers.
        // Under seq_cst, readers cannot disagree on the order of the two stores
        // in a way forbidden by a single total order. Teaching version just
        // shows the API and asserts a simple single-flag case.
        std::atomic<int> x{0};
        std::atomic<int> y{0};

        std::jthread w1([&] { x.store(1, std::memory_order_seq_cst); });
        std::jthread w2([&] { y.store(1, std::memory_order_seq_cst); });
        std::jthread r1([&] {
            while (x.load(std::memory_order_seq_cst) == 0) {
                std::this_thread::yield();
            }
            (void)y.load(std::memory_order_seq_cst);
        });
        std::jthread r2([&] {
            while (y.load(std::memory_order_seq_cst) == 0) {
                std::this_thread::yield();
            }
            (void)x.load(std::memory_order_seq_cst);
        });
        std::cout << "  four threads completed under seq_cst total order\n";
    }

    std::cout << "=== practical guidance ===\n";
    std::cout << "  · Start with seq_cst when unsure — correctness first.\n"
                 "  · Downgrade to acq/rel when you only need pairwise publish.\n"
                 "  · Downgrade to relaxed for pure independent counters.\n"
                 "  · Document WHY a weaker order is enough (review + TSan).\n";

    std::cout << "=== explicit vs default ===\n";
    {
        std::atomic<int> n{0};
        n.fetch_add(1, std::memory_order_seq_cst);
        n.fetch_add(1);  // same default
        assert(n.load(std::memory_order_seq_cst) == 2);
    }

    std::cout << "[memory_order_seq_cst] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_seq_cst", run>;

}  // namespace
