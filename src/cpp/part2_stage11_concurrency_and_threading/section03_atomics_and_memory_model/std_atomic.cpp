// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 7.1 / 7.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : std_atomic
// Topic id : part2/stage11/section03/std_atomic
// Refs     : https://en.cppreference.com/w/cpp/atomic/atomic
//            ISO [atomics.types.generic]
//
// std::atomic<T>: indivisible ops + optional memory_order.
// Acceptance: lock-free counter with many threads.

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

    std::cout << "=== [atomic] store / load / exchange / RMW ===\n";
    {
        std::atomic<int> x{0};
        x.store(5);
        assert(x.load() == 5);
        const int old = x.exchange(10);
        assert(old == 5 && x.load() == 10);
        assert(x.fetch_add(3) == 10);
        assert(x.load() == 13);
        ++x;  // atomic pre-increment
        assert(x.load() == 14);
        x += 2;
        assert(x.load() == 16);
        std::cout << "  x=" << x.load() << '\n';
    }

    std::cout << "=== trap: a = a + 1 is NOT a single atomic RMW ===\n";
    {
        std::atomic<int> a{0};
        // a = a + 1;  // load, add, store — three steps; race if concurrent
        a.fetch_add(1);  // correct
        assert(a.load() == 1);
        std::cout << "  use fetch_add / ++a, never a = a + 1 under concurrency\n";
    }

    std::cout << "=== lock-free counter (acceptance) ===\n";
    {
        std::atomic<int> counter{0};
        constexpr int kThreads = 8;
        constexpr int kPerThread = 50'000;

        {
            std::vector<std::jthread> pool;
            for (int t = 0; t < kThreads; ++t) {
                pool.emplace_back([&counter] {
                    for (int i = 0; i < kPerThread; ++i) {
                        // relaxed: only total matters; no publish relation
                        counter.fetch_add(1, std::memory_order_relaxed);
                    }
                });
            }
        }
        assert(counter.load() == kThreads * kPerThread);
        std::cout << "  counter=" << counter.load() << " is_lock_free=" << std::boolalpha << counter.is_lock_free()
                  << '\n';
        static_assert(std::atomic<int>::is_always_lock_free);
    }

    std::cout << "=== atomic_flag spin-style (always lock-free) ===\n";
    {
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
        assert(!flag.test_and_set());  // was clear
        assert(flag.test_and_set());   // was set
        flag.clear();
        assert(!flag.test());  // C++20 test()
        std::cout << "  atomic_flag clear/test_and_set ok\n";
    }

    std::cout << "=== wait / notify (C++20) ===\n";
    {
        std::atomic<int> gate{0};
        std::jthread waiter([&] {
            gate.wait(0);  // block while value == 0
            assert(gate.load() == 1);
        });
        gate.store(1);
        gate.notify_one();
    }

    std::cout << "[std_atomic] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/std_atomic", run>;

}  // namespace
