// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : graceful_shutdown
// Topic id : part2/stage11/section04/graceful_shutdown
//
// Covers: cooperative graceful shutdown

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<bool> stop{false};
    std::atomic<int> work{0};
    std::thread worker([&] {
        while (!stop.load(std::memory_order_acquire)) {
            work.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    stop.store(true, std::memory_order_release);
    worker.join();
    assert(work.load() >= 1);
}

void demo_intermediate() {
    std::mutex m;
    std::condition_variable cv;
    bool stop = false;
    int processed = 0;
    std::thread worker([&] {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return stop; });
        processed = 1;
    });
    {
        std::lock_guard<std::mutex> g(m);
        stop = true;
    }
    cv.notify_one();
    worker.join();
    assert(processed == 1);
}

void demo_expert() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::atomic<int> n{0};
    {
        std::jthread worker([&](std::stop_token st) {
            while (!st.stop_requested()) {
                n.fetch_add(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
    assert(n.load() >= 1);
#else
    std::atomic<bool> stop{false};
    std::atomic<int> n{0};
    std::thread worker([&] {
        while (!stop.load()) {
            n.fetch_add(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    stop.store(true);
    worker.join();
    assert(n.load() >= 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section04/graceful_shutdown", run>;

}  // namespace
