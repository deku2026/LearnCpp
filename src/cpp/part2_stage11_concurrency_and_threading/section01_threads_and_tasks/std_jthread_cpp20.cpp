// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 1.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_jthread_cpp20
// Topic id : part2/stage11/section01/std_jthread_cpp20
// Refs     : https://en.cppreference.com/w/cpp/thread/jthread
//            P0660 · ISO [thread.jthread.class]
//
// std::jthread (C++20): RAII join + built-in stop_source.
// Prefer over std::thread for almost all new code.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [std_jthread] RAII auto-join (no manual join) ===\n";
    {
        std::atomic<int> hits{0};
        {
            std::jthread t([&hits] {
                hits.fetch_add(1, std::memory_order_relaxed);
                std::cout << "  jthread body ran\n";
            });
            // leaving scope → request_stop() + join() automatically
        }
        assert(hits.load() == 1);
    }

    std::cout << "=== stop_token as first parameter (auto-injected) ===\n";
    {
        std::atomic<int> ticks{0};
        {
            std::jthread worker([&ticks](std::stop_token st) {
                while (!st.stop_requested()) {
                    ticks.fetch_add(1, std::memory_order_relaxed);
                    std::this_thread::sleep_for(20ms);
                }
                std::cout << "  cooperative exit after stop\n";
            });
            std::this_thread::sleep_for(80ms);
            // dtor: request_stop then join
        }
        assert(ticks.load() >= 2);
        std::cout << "  ticks=" << ticks.load() << '\n';
    }

    std::cout << "=== explicit request_stop + get_stop_token ===\n";
    {
        std::jthread worker([](std::stop_token st) {
            while (!st.stop_requested()) {
                std::this_thread::sleep_for(10ms);
            }
        });
        assert(worker.get_stop_token().stop_possible());
        assert(!worker.get_stop_token().stop_requested());
        const bool first = worker.request_stop();
        assert(first);
        assert(worker.get_stop_token().stop_requested());
        const bool second = worker.request_stop();  // idempotent
        assert(!second);
        // still joinable until dtor; explicit join also fine
        worker.join();
        assert(!worker.joinable());
    }

    std::cout << "=== pool of jthreads (vector dtor joins all) ===\n";
    {
        std::atomic<int> sum{0};
        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < 4; ++i) {
                pool.emplace_back([&sum, i] { sum.fetch_add(i, std::memory_order_relaxed); });
            }
        }  // all joined
        assert(sum.load() == 0 + 1 + 2 + 3);
    }

    std::cout << "[std_jthread_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_jthread_cpp20", run>;

}  // namespace
