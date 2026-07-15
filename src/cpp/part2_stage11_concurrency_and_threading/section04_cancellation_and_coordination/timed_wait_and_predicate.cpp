// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : timed_wait_and_predicate
// Topic id : part2/stage11/section04/timed_wait_and_predicate
//
// Covers: wait_for + predicate

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    std::thread t([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        {
            std::lock_guard<std::mutex> g(m);
            ready = true;
        }
        cv.notify_one();
    });
    {
        std::unique_lock<std::mutex> lk(m);
        const bool ok = cv.wait_for(lk, std::chrono::milliseconds(200), [&] { return ready; });
        assert(ok);
        assert(ready);
    }
    t.join();
}

void demo_intermediate() {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    {
        std::unique_lock<std::mutex> lk(m);
        const bool ok = cv.wait_for(lk, std::chrono::milliseconds(10), [&] { return ready; });
        assert(!ok);
    }
}

void demo_expert() {
    std::mutex m;
    std::condition_variable cv;
    int state = 0;
    std::thread t([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        {
            std::lock_guard<std::mutex> g(m);
            state = 1;
        }
        cv.notify_one();
    });
    {
        std::unique_lock<std::mutex> lk(m);
        while (state == 0) {
            (void)cv.wait_for(lk, std::chrono::milliseconds(50));
        }
        assert(state == 1);
    }
    t.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section04/timed_wait_and_predicate", run>;

}  // namespace
