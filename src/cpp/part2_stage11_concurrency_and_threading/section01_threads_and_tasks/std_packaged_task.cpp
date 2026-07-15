// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_packaged_task
// Topic id : part2/stage11/section01/std_packaged_task
//
// Covers: std::packaged_task + future

#include "learn/topic_registry.hpp"

#include <cassert>
#include <future>
#include <stdexcept>
#include <thread>
#include <utility>

namespace {

void demo_basics() {
    std::packaged_task<int()> task([] { return 40 + 2; });
    std::future<int> f = task.get_future();
    task();
    assert(f.get() == 42);
}

void demo_intermediate() {
    std::packaged_task<int(int, int)> task([](int a, int b) { return a * b; });
    std::future<int> f = task.get_future();
    std::thread t(std::move(task), 6, 7);
    assert(f.get() == 42);
    t.join();
}

void demo_expert() {
    std::packaged_task<int()> task([] {
        throw std::runtime_error("boom");
        return 0;
    });
    std::future<int> f = task.get_future();
    task();
    bool threw = false;
    try {
        (void)f.get();
    } catch (const std::runtime_error&) {
        threw = true;
    }
    assert(threw);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_packaged_task", run>;

}  // namespace
