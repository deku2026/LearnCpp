// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_async_future_promise
// Topic id : part2/stage11/section01/std_async_future_promise
//
// Covers: std::async, future, promise

#include "learn/topic_registry.hpp"

#include <future>
#include <stdexcept>
#include <thread>
#include <utility>

namespace {

void demo_basics() {
    std::future<int> f = std::async(std::launch::async, [] { return 21 * 2; });
    LEARN_CHECK(f.get() == 42);
}

void demo_intermediate() {
    std::promise<int> p;
    std::future<int> f = p.get_future();
    std::thread t([&p] { p.set_value(99); });
    LEARN_CHECK(f.get() == 99);
    t.join();
}

void demo_expert() {
    std::promise<void> ready;
    std::future<void> ready_f = ready.get_future();
    std::promise<int> result;
    std::future<int> result_f = result.get_future();

    std::thread producer([&] {
        ready_f.wait();
        result.set_value(7);
    });
    ready.set_value();
    LEARN_CHECK(result_f.get() == 7);
    producer.join();

    auto f1 = std::async(std::launch::async, [] { return 1; });
    auto f2 = std::async(std::launch::async, [] { return 2; });
    LEARN_CHECK(f1.get() + f2.get() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_async_future_promise", run>;

}  // namespace
