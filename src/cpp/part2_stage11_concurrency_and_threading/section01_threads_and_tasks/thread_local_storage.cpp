// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 1.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : thread_local_storage
// Topic id : part2/stage11/section01/thread_local_storage
// Refs     : https://en.cppreference.com/w/cpp/language/storage_duration
//            ISO [basic.stc.thread]
//
// thread_local: one independent instance per thread — no sharing, no race
// on that object. Great for per-thread RNG, buffers, error codes.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace {

thread_local int tls_counter = 0;

void bump_tls(int times) {
    for (int i = 0; i < times; ++i) {
        ++tls_counter;  // mutates THIS thread's copy only
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [thread_local] independent copies per thread ===\n";
    {
        tls_counter = 0;
        bump_tls(3);
        assert(tls_counter == 3);
        std::cout << "  main tls_counter=" << tls_counter << '\n';

        std::atomic<int> worker_seen{0};
        {
            std::jthread t([&worker_seen] {
                // Fresh default-initialized copy for this thread.
                assert(tls_counter == 0);
                bump_tls(5);
                assert(tls_counter == 5);
                worker_seen.store(tls_counter, std::memory_order_release);
            });
        }
        assert(worker_seen.load(std::memory_order_acquire) == 5);
        // Main thread's copy is untouched by the worker.
        assert(tls_counter == 3);
        std::cout << "  worker had 5; main still " << tls_counter << '\n';
    }

    std::cout << "=== many threads, each with private tls ===\n";
    {
        std::atomic<int> sum_of_private{0};
        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < 4; ++i) {
                pool.emplace_back([&sum_of_private, i] {
                    tls_counter = 0;
                    bump_tls(i + 1);  // 1,2,3,4
                    sum_of_private.fetch_add(tls_counter, std::memory_order_relaxed);
                });
            }
        }
        assert(sum_of_private.load() == 1 + 2 + 3 + 4);
        std::cout << "  sum of private counters=" << sum_of_private.load() << '\n';
    }

    std::cout << "=== use cases / caveats ===\n";
    std::cout << "  use: per-thread RNG engines, scratch buffers, errno-like state\n";
    std::cout << "  caveats: ctor/dtor run on first use / thread exit; "
                 "do not take address of tls and share across threads "
                 "without external sync\n";

    std::cout << "[thread_local_storage] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/thread_local_storage", run>;

}  // namespace
