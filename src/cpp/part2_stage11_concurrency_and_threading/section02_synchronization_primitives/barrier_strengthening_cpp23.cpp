// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 6.2 (C++23 收紧)
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : barrier_strengthening_cpp23
// Topic id : part2/stage11/section02/barrier_strengthening_cpp23
// Refs     : https://en.cppreference.com/w/cpp/thread/barrier
//            P2588 / C++23 DR: barrier phase completion guarantees
//            feature-test: __cpp_lib_barrier
//
// C++23 strengthens the happens-before / completion-step guarantees around
// barrier phase completion so that effects of the completion function are
// properly ordered w.r.t. threads that leave the phase.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <barrier>
#include <cassert>
#include <iostream>
#include <thread>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [barrier C++23] completion function visibility ===\n";
#if defined(__cpp_lib_barrier)
    std::cout << "  __cpp_lib_barrier=" << __cpp_lib_barrier << '\n';
#endif

    {
        // Shared non-atomic data published by the completion function, then
        // consumed by workers after arrive_and_wait returns. Under the C++23
        // strengthened model, completion effects happen-before threads proceed.
        int published = 0;
        std::atomic<int> observed_sum{0};

        constexpr int N = 4;
        std::barrier sync(N, [&published]() noexcept {
            // Runs once per phase when the last participant arrives.
            published += 10;
        });

        {
            std::vector<std::jthread> workers;
            for (int i = 0; i < N; ++i) {
                workers.emplace_back([&] {
                    for (int phase = 0; phase < 3; ++phase) {
                        sync.arrive_and_wait();
                        // After wait returns, completion for this phase has run.
                        observed_sum.fetch_add(published, std::memory_order_relaxed);
                    }
                });
            }
        }

        // 3 phases * +10 each → published ends at 30.
        // Each of N threads observes after each phase:
        // phase1: 10, phase2: 20, phase3: 30 → per thread 60, total 60*N.
        assert(published == 30);
        assert(observed_sum.load() == 60 * N);
        std::cout << "  published=" << published << " observed_sum=" << observed_sum.load() << '\n';
    }

    std::cout << "=== completion must be noexcept-invocable ===\n";
    {
        std::atomic<int> n{0};
        auto completion = [&n]() noexcept { n.fetch_add(1, std::memory_order_relaxed); };
        static_assert(std::is_nothrow_invocable_v<decltype(completion)&>);

        std::barrier bar(1, completion);
        bar.arrive_and_wait();
        assert(n.load() == 1);
        std::cout << "  noexcept completion invoked once\n";
    }

    std::cout << "[barrier_strengthening_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/barrier_strengthening_cpp23", run>;

}  // namespace
