// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : concurrency_headers
// Topic id : part3/section08/concurrency_headers
//
// Covers: headers map: thread mutex atomic future condition_variable

#include "learn/topic_registry.hpp"

#include <atomic>
#include <future>
#include <mutex>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.fetch_add(1);
    LEARN_CHECK(x.load() == 1);
}

void demo_intermediate() {
    std::mutex m;
    int n = 0;
    {
        std::lock_guard<std::mutex> lock(m);
        ++n;
    }
    LEARN_CHECK(n == 1);
}

void demo_expert() {
    std::promise<int> pr;
    std::future<int> fut = pr.get_future();
    std::thread th([&] { pr.set_value(42); });
    LEARN_CHECK(fut.get() == 42);
    th.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/concurrency_headers", run>;

}  // namespace
