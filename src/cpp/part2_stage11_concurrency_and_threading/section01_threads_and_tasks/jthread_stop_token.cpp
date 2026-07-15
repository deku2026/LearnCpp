// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 1.2 / 2 / 验收点
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : jthread_stop_token
// Topic id : part2/stage11/section01/jthread_stop_token
// Refs     : https://en.cppreference.com/w/cpp/thread/jthread
//            https://en.cppreference.com/w/cpp/thread/stop_token
//
// Acceptance: cancellable worker via jthread + stop_token.
// Cancellation is cooperative — never force-kill a standard thread.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <stop_token>
#include <thread>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [jthread_stop_token] graceful cancel loop ===\n";
    {
        std::atomic<int> work_units{0};
        std::atomic<bool> exited_cleanly{false};

        {
            std::jthread worker([&](std::stop_token st) {
                // Optional: react immediately when stop is requested.
                std::stop_callback on_stop(st, [] {
                    // Keep callbacks short & non-blocking.
                    // (Printing here is only for the teaching demo.)
                });

                while (!st.stop_requested()) {
                    // "safe point": check between units of work
                    work_units.fetch_add(1, std::memory_order_relaxed);
                    std::this_thread::sleep_for(15ms);
                }
                // Clean up resources here — this is the cooperative exit.
                exited_cleanly.store(true, std::memory_order_release);
            });

            std::this_thread::sleep_for(70ms);
            const bool requested = worker.request_stop();
            assert(requested);
            // dtor also request_stop+join if we did not join; we join explicitly
            worker.join();
        }

        assert(exited_cleanly.load(std::memory_order_acquire));
        assert(work_units.load() >= 2);
        std::cout << "  work_units=" << work_units.load() << " exited_cleanly=true\n";
    }

    std::cout << "=== scope-driven cancel (no manual request_stop) ===\n";
    {
        std::atomic<int> n{0};
        {
            std::jthread background([&n](std::stop_token st) {
                while (!st.stop_requested()) {
                    n.fetch_add(1, std::memory_order_relaxed);
                    std::this_thread::sleep_for(10ms);
                }
            });
            std::this_thread::sleep_for(35ms);
            // leave scope → auto request_stop + join
        }
        assert(n.load() >= 1);
        std::cout << "  n after auto-stop=" << n.load() << '\n';
    }

    std::cout << "=== note: blocking waits need condition_variable_any ===\n";
    std::cout << "  stop_requested alone cannot interrupt a bare cv.wait; "
                 "use wait(lock, stop_token, pred) or a stop_callback that "
                 "notifies the cv (see section04).\n";

    std::cout << "[jthread_stop_token] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/jthread_stop_token", run>;

}  // namespace
