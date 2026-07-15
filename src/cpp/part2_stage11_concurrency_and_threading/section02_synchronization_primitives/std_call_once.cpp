// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 5.4
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_call_once
// Topic id : part2/stage11/section02/std_call_once
// Refs     : https://en.cppreference.com/w/cpp/thread/call_once
//            ISO [thread.once]
//
// call_once + once_flag: exactly-once initialization under concurrency.
// Note: function-local static init is also thread-safe since C++11 (simpler
// Meyers singleton). call_once is for non-static / multi-callable scenarios.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [call_once] concurrent init runs once ===\n";
    {
        std::once_flag flag;
        std::atomic<int> init_count{0};
        std::atomic<int> use_count{0};
        int resource = 0;

        auto use = [&] {
            std::call_once(flag, [&] {
                init_count.fetch_add(1, std::memory_order_relaxed);
                resource = 123;
            });
            // After call_once returns, init is complete and visible.
            assert(resource == 123);
            use_count.fetch_add(1, std::memory_order_relaxed);
        };

        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < 16; ++i) {
                pool.emplace_back(use);
            }
        }

        assert(init_count.load() == 1);
        assert(use_count.load() == 16);
        std::cout << "  init_count=1 use_count=16 resource=" << resource << '\n';
    }

    std::cout << "=== exception in init: other threads retry ===\n";
    {
        std::once_flag flag;
        std::atomic<int> attempts{0};
        int value = 0;

        auto flaky_init = [&] {
            try {
                std::call_once(flag, [&] {
                    const int n = attempts.fetch_add(1, std::memory_order_relaxed);
                    if (n == 0) {
                        throw std::runtime_error("first init failed");
                    }
                    value = 7;
                });
            } catch (const std::runtime_error&) {
                // first caller may see the throw; others may retry
            }
        };

        {
            std::jthread t1(flaky_init);
            std::jthread t2(flaky_init);
            std::jthread t3(flaky_init);
        }
        // Eventually one successful call_once completes.
        std::call_once(flag, [&] { value = 7; });
        assert(value == 7);
        std::cout << "  recovered value=" << value << " attempts>=" << attempts.load() << '\n';
    }

    std::cout << "alternative: function-local static for singleton init\n";
    std::cout << "[std_call_once] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_call_once", run>;

}  // namespace
