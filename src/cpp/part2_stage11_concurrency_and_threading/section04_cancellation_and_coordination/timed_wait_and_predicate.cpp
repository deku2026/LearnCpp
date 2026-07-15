// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 5.2 / 2.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : timed_wait_and_predicate
// Topic id : part2/stage11/section04/timed_wait_and_predicate
// Refs     : https://en.cppreference.com/w/cpp/thread/condition_variable
//            https://en.cppreference.com/w/cpp/thread/condition_variable_any
//
// Timed waits + predicates: wait_for / wait_until always re-check the
// condition (spurious wakeups + timeouts). Prefer predicate overloads.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [timed wait] wait_for with predicate ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;

        std::jthread producer([&] {
            std::this_thread::sleep_for(25ms);
            {
                std::lock_guard lock(mtx);
                ready = true;
            }
            cv.notify_one();
        });

        std::unique_lock lock(mtx);
        // Returns whether predicate is true (not merely whether notified).
        const bool ok = cv.wait_for(lock, 500ms, [&] { return ready; });
        assert(ok);
        assert(ready);
        std::cout << "  predicate true before timeout\n";
    }

    std::cout << "=== timeout path: predicate still false ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;

        std::unique_lock lock(mtx);
        const bool ok = cv.wait_for(lock, 20ms, [&] { return ready; });
        assert(!ok);
        assert(!ready);
        std::cout << "  wait_for returned false on timeout\n";
    }

    std::cout << "=== wait_until absolute deadline ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;

        const auto deadline = std::chrono::steady_clock::now() + 15ms;
        std::unique_lock lock(mtx);
        const bool ok = cv.wait_until(lock, deadline, [&] { return ready; });
        assert(!ok);
        std::cout << "  wait_until deadline expired cleanly\n";
    }

    std::cout << "=== polling stop_token with wait_for slices ===\n";
    {
        // Alternative when you cannot use condition_variable_any::wait(st):
        // short timed waits that re-check stop_requested.
        std::mutex mtx;
        std::condition_variable cv;
        bool data_ready = false;
        std::atomic<bool> saw_stop{false};

        std::jthread worker([&](std::stop_token st) {
            std::unique_lock lock(mtx);
            while (!data_ready) {
                if (st.stop_requested()) {
                    saw_stop.store(true, std::memory_order_relaxed);
                    return;
                }
                cv.wait_for(lock, 10ms, [&] { return data_ready || st.stop_requested(); });
            }
        });

        std::this_thread::sleep_for(30ms);
        worker.request_stop();
        cv.notify_all();
        worker.join();
        assert(saw_stop.load());
        std::cout << "  timed slices observed stop_requested\n";
    }

    std::cout << "iron rule: never bare wait/wait_for without a predicate\n";
    std::cout << "[timed_wait_and_predicate] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section04/timed_wait_and_predicate", run>;

}  // namespace
