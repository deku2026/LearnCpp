// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_mutex
// Topic id : part2/stage11/section02/std_mutex
//
// Covers: std::mutex critical section

#include "learn/topic_registry.hpp"

#include <mutex>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
    std::mutex m;
    int counter = 0;
    auto inc = [&] {
        for (int i = 0; i < 1000; ++i) {
            m.lock();
            ++counter;
            m.unlock();
        }
    };
    std::thread t1(inc);
    std::thread t2(inc);
    t1.join();
    t2.join();
    LEARN_CHECK(counter == 2000);
}

void demo_intermediate() {
    std::mutex m;
    std::vector<int> v;
    auto push = [&](int x) {
        std::lock_guard<std::mutex> g(m);
        v.push_back(x);
    };
    std::thread t1([&] { push(1); });
    std::thread t2([&] { push(2); });
    t1.join();
    t2.join();
    LEARN_CHECK(v.size() == 2);
}

void demo_expert() {
    std::mutex m;
    bool locked = m.try_lock();
    LEARN_CHECK(locked);
    m.unlock();
    m.lock();
    bool second = m.try_lock();
    LEARN_CHECK(!second);
    m.unlock();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_mutex", run>;

}  // namespace
