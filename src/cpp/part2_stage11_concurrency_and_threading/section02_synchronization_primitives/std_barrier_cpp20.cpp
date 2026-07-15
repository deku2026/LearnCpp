// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 6.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_barrier_cpp20
// Topic id : part2/stage11/section02/std_barrier_cpp20
// Refs     : https://en.cppreference.com/w/cpp/thread/barrier
//            P1135 · ISO [thread.coord.barrier]
//
// barrier (C++20): reusable multi-phase rendezvous. Optional completion
// function runs once each time the expected count arrives (must be noexcept).

#include "learn/topic_registry.hpp"

#include <atomic>
#include <barrier>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [barrier] multi-phase parallel steps ===\n";
    {
        constexpr int N = 3;
        constexpr int kPhases = 3;
        std::atomic<int> completions{0};
        std::atomic<int> phase_hits{0};

        std::barrier sync(N, [&completions]() noexcept { completions.fetch_add(1, std::memory_order_relaxed); });

        {
            std::vector<std::jthread> workers;
            for (int i = 0; i < N; ++i) {
                workers.emplace_back([&sync, &phase_hits] {
                    for (int p = 0; p < kPhases; ++p) {
                        // ... phase work ...
                        phase_hits.fetch_add(1, std::memory_order_relaxed);
                        sync.arrive_and_wait();  // all N must arrive each phase
                    }
                });
            }
        }

        assert(phase_hits.load() == N * kPhases);
        assert(completions.load() == kPhases);
        std::cout << "  phases=" << kPhases << " completion calls=" << completions.load() << '\n';
    }

    std::cout << "=== arrive_and_drop reduces expected for later phases ===\n";
    {
        std::atomic<int> steps{0};
        std::barrier bar(2);
        {
            std::jthread temporary([&] {
                steps.fetch_add(1, std::memory_order_relaxed);
                bar.arrive_and_drop();  // this thread leaves the barrier group
            });
            bar.arrive_and_wait();  // phase 0 with expected=2
            // phase 1 now expects 1 participant
            bar.arrive_and_wait();
            steps.fetch_add(1, std::memory_order_relaxed);
        }
        assert(steps.load() == 2);
        std::cout << "  drop then single-thread phase ok\n";
    }

    std::cout << "vs latch: barrier resets; latch is one-shot\n";
    std::cout << "[std_barrier_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_barrier_cpp20", run>;

}  // namespace
