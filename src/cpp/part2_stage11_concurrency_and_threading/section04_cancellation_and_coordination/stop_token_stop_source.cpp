// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 2.1
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : stop_token_stop_source
// Topic id : part2/stage11/section04/stop_token_stop_source
// Refs     : https://en.cppreference.com/w/cpp/thread/stop_token
//            https://en.cppreference.com/w/cpp/thread/stop_source
//            https://en.cppreference.com/w/cpp/thread/stop_callback
//            P0660 · ISO [thread.stoptoken]
//
// Cooperative cancel trio: stop_source (request) / stop_token (observe) /
// stop_callback (react). Never force-kill standard threads.

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

    std::cout << "=== [stop_source / stop_token] request side ===\n";
    {
        std::stop_source source;
        std::stop_token token = source.get_token();
        assert(token.stop_possible());
        assert(!token.stop_requested());

        std::atomic<int> ticks{0};
        std::jthread worker([&token, &ticks] {
            while (!token.stop_requested()) {
                ticks.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::sleep_for(10ms);
            }
        });

        std::this_thread::sleep_for(35ms);
        assert(source.request_stop());   // first request succeeds
        assert(!source.request_stop());  // subsequent are no-ops
        assert(token.stop_requested());
        worker.join();
        assert(ticks.load() >= 1);
        std::cout << "  ticks=" << ticks.load() << " after request_stop\n";
    }

    std::cout << "=== stop_callback fires on request ===\n";
    {
        std::stop_source source;
        std::atomic<bool> cb_ran{false};

        std::stop_callback cb(source.get_token(), [&] { cb_ran.store(true, std::memory_order_release); });

        assert(!cb_ran.load());
        source.request_stop();
        assert(cb_ran.load(std::memory_order_acquire));
        std::cout << "  callback invoked synchronously on request_stop\n";
    }

    std::cout << "=== callback if already stopped (runs immediately) ===\n";
    {
        std::stop_source source;
        source.request_stop();
        std::atomic<bool> ran{false};
        std::stop_callback cb(source.get_token(), [&] { ran.store(true, std::memory_order_relaxed); });
        assert(ran.load());
        std::cout << "  late-registered callback still runs\n";
    }

    std::cout << "=== share stop state via stop_source copy ===\n";
    {
        std::stop_source a;
        std::stop_source b = a;  // same stop-state
        std::stop_token t = b.get_token();
        a.request_stop();
        assert(t.stop_requested());
        std::cout << "  copies share one stop-state\n";
    }

    std::cout << "nostopstate: stop_source that cannot stop\n";
    {
        std::stop_source never{std::nostopstate};
        assert(!never.stop_possible());
        assert(!never.request_stop());
    }

    std::cout << "[stop_token_stop_source] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section04/stop_token_stop_source", run>;

}  // namespace
