// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 6.1
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_latch_cpp20
// Topic id : part2/stage11/section02/std_latch_cpp20
// Refs     : https://en.cppreference.com/w/cpp/thread/latch
//            P1135 · ISO [thread.coord.latch]
//
// latch (C++20): one-shot countdown. count_down / wait until zero; not reusable.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [latch] wait for N workers ===\n";
    {
        constexpr int N = 4;
        std::latch done(N);
        std::atomic<int> finished{0};

        {
            std::vector<std::jthread> workers;
            for (int i = 0; i < N; ++i) {
                workers.emplace_back([&done, &finished, i] {
                    finished.fetch_add(1, std::memory_order_relaxed);
                    (void)i;
                    done.count_down();  // may count_down(n) for multi-units
                });
            }
            done.wait();  // blocks until count hits 0
        }
        assert(finished.load() == N);
        assert(done.try_wait());  // already open
        std::cout << "  all " << N << " workers finished\n";
    }

    std::cout << "=== start gate: main opens, workers arrive_and_wait ===\n";
    {
        constexpr int N = 3;
        std::latch start_gate(1);  // main will release
        std::latch all_ready(N);   // workers signal ready
        std::atomic<int> ran{0};

        {
            std::vector<std::jthread> workers;
            for (int i = 0; i < N; ++i) {
                workers.emplace_back([&] {
                    all_ready.count_down();
                    start_gate.wait();  // wait for main's go
                    ran.fetch_add(1, std::memory_order_relaxed);
                });
            }
            all_ready.wait();  // all parked on start_gate
            assert(ran.load() == 0);
            start_gate.count_down();  // open the gate
        }
        assert(ran.load() == N);
        std::cout << "  simultaneous start of " << N << " workers\n";
    }

    std::cout << "=== arrive_and_wait = count_down + wait ===\n";
    {
        std::latch sync(2);
        std::atomic<int> after{0};
        {
            std::jthread t([&] {
                sync.arrive_and_wait();
                after.fetch_add(1, std::memory_order_relaxed);
            });
            sync.arrive_and_wait();
            after.fetch_add(1, std::memory_order_relaxed);
        }
        assert(after.load() == 2);
    }

    std::cout << "latch is one-shot; use barrier for multi-phase reuse\n";
    std::cout << "[std_latch_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_latch_cpp20", run>;

}  // namespace
