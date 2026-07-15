// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : thread_local_storage
// Topic id : part2/stage11/section01/thread_local_storage
//
// Covers: thread_local per-thread copies

#include "learn/topic_registry.hpp"

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace {

thread_local int tls_counter = 0;

void demo_basics() {
    tls_counter = 5;
    LEARN_CHECK(tls_counter == 5);
    std::thread t([] {
        LEARN_CHECK(tls_counter == 0);
        tls_counter = 9;
        LEARN_CHECK(tls_counter == 9);
    });
    t.join();
    LEARN_CHECK(tls_counter == 5);
}

void demo_intermediate() {
    std::mutex m;
    std::vector<int> seen;
    auto work = [&] {
        tls_counter = 0;
        ++tls_counter;
        ++tls_counter;
        std::lock_guard<std::mutex> g(m);
        seen.push_back(tls_counter);
    };
    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();
    LEARN_CHECK(seen.size() == 2);
    LEARN_CHECK(seen[0] == 2 && seen[1] == 2);
}

void demo_expert() {
    thread_local int* p = nullptr;
    int local = 1;
    p = &local;
    LEARN_CHECK(*p == 1);
    std::atomic<int> other{0};
    std::thread t([&] {
        LEARN_CHECK(p == nullptr);
        int x = 7;
        p = &x;
        other.store(*p);
    });
    t.join();
    LEARN_CHECK(other.load() == 7);
    LEARN_CHECK(*p == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/thread_local_storage", run>;

}  // namespace
