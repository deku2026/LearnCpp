// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : jthread_stop_token
// Topic id : part2/stage11/section01/jthread_stop_token
//
// Covers: jthread stop_token cooperative cancel

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<int> ticks{0};
    {
        std::jthread worker([&](std::stop_token st) {
            while (!st.stop_requested()) {
                ticks.fetch_add(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    assert(ticks.load() >= 1);
#else
    std::atomic<bool> stop{false};
    std::atomic<int> ticks{0};
    std::thread worker([&] {
        while (!stop.load()) {
            ticks.fetch_add(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    stop.store(true);
    worker.join();
    assert(ticks.load() >= 1);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<bool> saw_stop{false};
    {
        std::jthread worker([&](std::stop_token st) {
            while (!st.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            saw_stop.store(true);
        });
        worker.request_stop();
    }
    assert(saw_stop.load());
#else
    std::atomic<bool> stop{false};
    std::atomic<bool> saw_stop{false};
    std::thread worker([&] {
        while (!stop.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        saw_stop.store(true);
    });
    stop.store(true);
    worker.join();
    assert(saw_stop.load());
#endif
}

void demo_expert() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<int> cleaned{0};
    {
        std::jthread worker([&](std::stop_token st) {
            std::stop_callback cb(st, [&] { cleaned.fetch_add(1); });
            while (!st.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    assert(cleaned.load() == 1);
#else
    std::atomic<int> cleaned{0};
    std::atomic<bool> stop{false};
    std::thread worker([&] {
        while (!stop.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        cleaned.fetch_add(1);
    });
    stop.store(true);
    worker.join();
    assert(cleaned.load() == 1);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/jthread_stop_token", run>;

}  // namespace
