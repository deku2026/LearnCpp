// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 7.5
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : atomic_ref_cpp20
// Topic id : part2/stage11/section03/atomic_ref_cpp20
// Refs     : https://en.cppreference.com/w/cpp/atomic/atomic_ref
//            ISO [atomics.ref.generic]
//
// atomic_ref (C++20): apply atomic ops to an existing non-atomic object.
// While any atomic_ref to an object exists, ALL accesses must go through
// atomic_ref (or be sequenced as if atomic) — else data race.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [atomic_ref] atomic ops on plain array element ===\n";
    {
        int data[8] = {};
        {
            std::vector<std::jthread> pool;
            for (int t = 0; t < 4; ++t) {
                pool.emplace_back([&data] {
                    for (int i = 0; i < 1000; ++i) {
                        std::atomic_ref<int> ref(data[3]);
                        ref.fetch_add(1, std::memory_order_relaxed);
                    }
                });
            }
        }
        assert(data[3] == 4000);
        std::cout << "  data[3]=" << data[3] << '\n';
    }

    std::cout << "=== lock-free query ===\n";
    {
        int x = 0;
        std::atomic_ref<int> ref(x);
        std::cout << "  is_lock_free=" << std::boolalpha << ref.is_lock_free()
                  << " is_always_lock_free=" << std::atomic_ref<int>::is_always_lock_free << '\n';
        ref.store(9);
        assert(ref.load() == 9);
        assert(x == 9);
    }

    std::cout << "=== do NOT mix plain access while atomic_ref is live ===\n";
    {
        int y = 0;
        {
            std::atomic_ref<int> ref(y);
            ref.store(1);
            // y = 2;  // ❌ data race if another thread uses atomic_ref concurrently
            assert(ref.load() == 1);
        }
        // After all atomic_ref lifetimes end, plain access is fine again
        // (single-threaded here).
        y = 2;
        assert(y == 2);
        std::cout << "  exclusive plain access only when no concurrent atomic_ref\n";
    }

    std::cout << "[atomic_ref_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/atomic_ref_cpp20", run>;

}  // namespace
