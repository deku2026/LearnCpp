// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : unique_lock
// Topic id : part2/stage11/section02/unique_lock
//
// Covers: std::unique_lock flexible ownership

#include "learn/topic_registry.hpp"

#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::mutex m;
    std::unique_lock<std::mutex> lk(m);
    LEARN_CHECK(lk.owns_lock());
    lk.unlock();
    LEARN_CHECK(!lk.owns_lock());
}

void demo_intermediate() {
    std::mutex m;
    int v = 0;
    std::unique_lock<std::mutex> lk(m, std::defer_lock);
    LEARN_CHECK(!lk.owns_lock());
    lk.lock();
    v = 5;
    lk.unlock();
    LEARN_CHECK(v == 5);
}

void demo_expert() {
    std::mutex m;
    int shared = 0;
    auto work = [&] {
        std::unique_lock<std::mutex> lk(m);
        ++shared;
        lk.unlock();
        lk.lock();
        shared += 10;
    };
    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();
    LEARN_CHECK(shared == 22);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/unique_lock", run>;

}  // namespace
