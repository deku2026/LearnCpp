// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 7.2 / 8.2 / 8.4
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_relaxed
// Topic id : part2/stage11/section03/memory_order_relaxed
// Refs     : https://en.cppreference.com/w/cpp/atomic/memory_order
//            ISO [atomics.order]
//
// relaxed: atomicity only — no inter-thread ordering/visibility beyond that.
// Correct for independent counters/stats; WRONG as a "data ready" flag.

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

    std::cout << "=== [relaxed] independent counter — order is enough ===\n";
    {
        std::atomic<long long> hits{0};
        {
            std::vector<std::jthread> pool;
            for (int t = 0; t < 6; ++t) {
                pool.emplace_back([&hits] {
                    for (int i = 0; i < 20'000; ++i) {
                        hits.fetch_add(1, std::memory_order_relaxed);
                    }
                });
            }
        }
        assert(hits.load(std::memory_order_relaxed) == 6 * 20'000);
        std::cout << "  hits=" << hits.load() << '\n';
        std::cout << "  why relaxed suffices: only the final total matters;\n"
                     "  no other data is published through this atomic;\n"
                     "  RMW atomicity alone prevents lost updates.\n";
    }

    std::cout << "=== multiple independent relaxed metrics ===\n";
    {
        std::atomic<int> a{0};
        std::atomic<int> b{0};
        {
            std::jthread t1([&] {
                for (int i = 0; i < 1000; ++i) {
                    a.fetch_add(1, std::memory_order_relaxed);
                }
            });
            std::jthread t2([&] {
                for (int i = 0; i < 1000; ++i) {
                    b.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        assert(a.load() == 1000 && b.load() == 1000);
        std::cout << "  a and b are separate; no cross-metric ordering needed\n";
    }

    std::cout << "=== when relaxed is NOT enough ===\n";
    std::cout << "  Publishing non-atomic data behind a flag needs\n"
                 "  release store + acquire load (see memory_order_acquire_release).\n"
                 "  relaxed flag can make a consumer see ready=true with stale data.\n";

    std::cout << "[memory_order_relaxed] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_relaxed", run>;

}  // namespace
