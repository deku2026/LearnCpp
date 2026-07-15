// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 8.2 / 8.4
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_acq_rel
// Topic id : part2/stage11/section03/memory_order_acq_rel
// Refs     : https://en.cppreference.com/w/cpp/atomic/memory_order
//
// memory_order_acq_rel: for RMW ops only — acquire on the load half,
// release on the store half. Typical on CAS / fetch_add that both consume
// prior publishes and publish new state.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [acq_rel] RMW as both consumer and publisher ===\n";
    {
        // Message slots: writers fill payload[i], then CAS-claim index.
        // Readers wait until index advances, then read that slot.
        constexpr int N = 4;
        int payload[N] = {};
        std::atomic<int> published{0};  // count of ready slots

        auto writer = [&](int id) {
            payload[id] = 100 + id;  // non-atomic fill
            // fetch_add acq_rel: release publishes payload[id];
            // acquire observes earlier writers if needed for chaining.
            published.fetch_add(1, std::memory_order_acq_rel);
        };

        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < N; ++i) {
                pool.emplace_back(writer, i);
            }
        }

        // Main acts as consumer: acquire-load the counter.
        while (published.load(std::memory_order_acquire) < N) {
            std::this_thread::yield();
        }
        for (int i = 0; i < N; ++i) {
            assert(payload[i] == 100 + i);
        }
        std::cout << "  all " << N << " payloads visible after acquire\n";
    }

    std::cout << "=== CAS with acq_rel success / relaxed failure ===\n";
    {
        std::atomic<int> state{0};
        int expected = 0;
        // success: acq_rel; failure: relaxed load of current
        [[maybe_unused]] const bool ok =
            state.compare_exchange_strong(expected, 1, std::memory_order_acq_rel, std::memory_order_relaxed);
        assert(ok && state.load() == 1);

        expected = 0;
        [[maybe_unused]] const bool fail =
            state.compare_exchange_strong(expected, 2, std::memory_order_acq_rel, std::memory_order_relaxed);
        assert(!fail && expected == 1);
        std::cout << "  asymmetric success/failure orders are common on CAS\n";
    }

    std::cout << "=== when to pick acq_rel ===\n";
    std::cout << "  RMW that both (1) must see prior release-published data and\n"
                 "  (2) itself publishes to later acquire readers → acq_rel.\n"
                 "  Pure counter with no publish relation → relaxed is enough.\n";

    std::cout << "[memory_order_acq_rel] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_acq_rel", run>;

}  // namespace
