// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 3.1 / 3.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_async_future_promise
// Topic id : part2/stage11/section01/std_async_future_promise
// Refs     : https://en.cppreference.com/w/cpp/thread/async
//            https://en.cppreference.com/w/cpp/thread/future
//            https://en.cppreference.com/w/cpp/thread/promise
//            ISO [futures]
//
// async/future: "compute a value later". promise: manually set that value
// (or exception) from another thread.

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

int compute_answer() {
    return 42;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [async] launch::async — real new thread ===\n";
    {
        // Prefer explicit launch::async; default may be deferred (lazy).
        std::future<int> fut = std::async(std::launch::async, compute_answer);
        // Do other work here...
        const int result = fut.get();  // blocks until ready; once only
        assert(result == 42);
        std::cout << "  result=" << result << '\n';
    }

    std::cout << "=== parallel partial sums with async ===\n";
    {
        std::vector<int> data(1000);
        std::iota(data.begin(), data.end(), 1);  // 1..1000, sum=500500

        auto sum_range = [&data](std::size_t lo, std::size_t hi) {
            return std::accumulate(data.begin() + static_cast<std::ptrdiff_t>(lo),
                                   data.begin() + static_cast<std::ptrdiff_t>(hi), 0);
        };

        auto f0 = std::async(std::launch::async, sum_range, 0u, 250u);
        auto f1 = std::async(std::launch::async, sum_range, 250u, 500u);
        auto f2 = std::async(std::launch::async, sum_range, 500u, 750u);
        auto f3 = std::async(std::launch::async, sum_range, 750u, 1000u);
        // MUST hold futures: temporary future dtor blocks → accidental serial
        const int total = f0.get() + f1.get() + f2.get() + f3.get();
        assert(total == 500500);
        std::cout << "  total=" << total << '\n';
    }

    std::cout << "=== deferred policy (lazy, runs on get() thread) ===\n";
    {
        std::future<int> lazy = std::async(std::launch::deferred, [] { return 7; });
        assert(lazy.wait_for(0ms) == std::future_status::deferred);
        assert(lazy.get() == 7);
    }

    std::cout << "=== promise: set_value from producer thread ===\n";
    {
        std::promise<int> prom;
        std::future<int> fut = prom.get_future();

        std::jthread producer([&prom] {
            std::this_thread::sleep_for(10ms);
            prom.set_value(99);
        });

        assert(fut.get() == 99);
        std::cout << "  received 99\n";
    }

    std::cout << "=== promise: set_exception across threads ===\n";
    {
        std::promise<int> prom;
        std::future<int> fut = prom.get_future();

        std::jthread producer([&prom] {
            try {
                throw std::runtime_error("boom from worker");
            } catch (...) {
                prom.set_exception(std::current_exception());
            }
        });

        [[maybe_unused]] bool caught = false;
        try {
            (void)fut.get();
        } catch (const std::runtime_error& e) {
            caught = true;
            std::cout << "  rethrown: " << e.what() << '\n';
        }
        assert(caught);
    }

    std::cout << "=== wait_for readiness ===\n";
    {
        auto fut = std::async(std::launch::async, [] {
            std::this_thread::sleep_for(30ms);
            return 1;
        });
        [[maybe_unused]] const auto st = fut.wait_for(1ms);
        assert(st == std::future_status::timeout || st == std::future_status::ready);
        assert(fut.get() == 1);
    }

    std::cout << "[std_async_future_promise] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_async_future_promise", run>;

}  // namespace
