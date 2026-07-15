// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 6.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_counting_semaphore_cpp20
// Topic id : part2/stage11/section02/std_counting_semaphore_cpp20
// Refs     : https://en.cppreference.com/w/cpp/thread/counting_semaphore
//            ISO [thread.sema.cnt]
//
// counting_semaphore: pool of N permits. acquire blocks when empty;
// release returns permits. Unlike mutex, different threads may acquire/release.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [counting_semaphore] limit concurrency to 3 ===\n";
    {
        constexpr int kPermits = 3;
        std::counting_semaphore<kPermits> pool(kPermits);
        std::atomic<int> in_critical{0};
        std::atomic<int> max_seen{0};
        std::atomic<int> completed{0};

        auto worker = [&] {
            pool.acquire();
            const int now = in_critical.fetch_add(1, std::memory_order_relaxed) + 1;
            // Track peak concurrency (relaxed is fine for a soft max metric).
            int prev = max_seen.load(std::memory_order_relaxed);
            while (now > prev && !max_seen.compare_exchange_weak(prev, now, std::memory_order_relaxed)) {
            }
            std::this_thread::sleep_for(5ms);
            in_critical.fetch_sub(1, std::memory_order_relaxed);
            pool.release();
            completed.fetch_add(1, std::memory_order_relaxed);
        };

        {
            std::vector<std::jthread> threads;
            for (int i = 0; i < 12; ++i) {
                threads.emplace_back(worker);
            }
        }

        assert(completed.load() == 12);
        assert(max_seen.load() <= kPermits);
        assert(max_seen.load() >= 1);
        std::cout << "  max concurrent=" << max_seen.load() << " (cap=" << kPermits << ")\n";
    }

    std::cout << "=== try_acquire / try_acquire_for ===\n";
    {
        std::counting_semaphore<2> sem(1);
        assert(sem.try_acquire());
        assert(!sem.try_acquire());  // empty
        assert(!sem.try_acquire_for(5ms));
        sem.release();
        assert(sem.try_acquire_for(50ms));
        sem.release();
        std::cout << "  timed try helpers ok\n";
    }

    std::cout << "semaphore ≠ mutex: any thread may release a permit\n";
    std::cout << "[std_counting_semaphore_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_counting_semaphore_cpp20", run>;

}  // namespace
