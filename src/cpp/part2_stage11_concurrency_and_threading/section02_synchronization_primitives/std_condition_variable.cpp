// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_condition_variable
// Topic id : part2/stage11/section02/std_condition_variable
//
// Covers: condition_variable producer-consumer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace {

void demo_basics() {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    int value = 0;
    std::thread producer([&] {
        {
            std::lock_guard<std::mutex> g(m);
            value = 42;
            ready = true;
        }
        cv.notify_one();
    });
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return ready; });
        assert(value == 42);
    }
    producer.join();
}

void demo_intermediate() {
    std::mutex m;
    std::condition_variable cv;
    std::queue<int> q;
    bool done = false;
    int sum = 0;
    std::thread consumer([&] {
        for (;;) {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [&] { return !q.empty() || done; });
            if (q.empty() && done) {
                break;
            }
            sum += q.front();
            q.pop();
        }
    });
    {
        std::lock_guard<std::mutex> g(m);
        q.push(1);
        q.push(2);
        q.push(3);
        done = true;
    }
    cv.notify_all();
    consumer.join();
    assert(sum == 6);
}

void demo_expert() {
    std::mutex m;
    std::condition_variable cv;
    int phase = 0;
    std::thread t([&] {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return phase >= 1; });
        phase = 2;
        lk.unlock();
        cv.notify_one();
    });
    {
        std::lock_guard<std::mutex> g(m);
        phase = 1;
    }
    cv.notify_one();
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return phase == 2; });
    }
    t.join();
    assert(phase == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_condition_variable", run>;

}  // namespace
