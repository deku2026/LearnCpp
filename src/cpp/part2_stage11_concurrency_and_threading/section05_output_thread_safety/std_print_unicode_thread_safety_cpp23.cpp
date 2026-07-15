// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : std_print_unicode_thread_safety_cpp23
// Topic id : part2/stage11/section05/std_print_unicode_thread_safety_cpp23
//
// Covers: print / synchronized message emission

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace {

void demo_basics() {
    // Portable assertion path: synchronized whole-message emission.
    std::mutex m;
    std::ostringstream oss;
    {
        std::lock_guard<std::mutex> g(m);
        oss << "ok";
    }
    assert(oss.str() == "ok");
}

void demo_intermediate() {
    std::mutex m;
    std::ostringstream oss;
    std::thread t1([&] {
        std::lock_guard<std::mutex> g(m);
        oss << "1";
    });
    std::thread t2([&] {
        std::lock_guard<std::mutex> g(m);
        oss << "2";
    });
    t1.join();
    t2.join();
    assert(oss.str().size() == 2);
}

void demo_expert() {
    std::atomic<int> messages{0};
    std::mutex m;
    std::ostringstream oss;
    auto emit = [&](const char* s) {
        std::lock_guard<std::mutex> g(m);
        oss << s;
        messages.fetch_add(1);
    };
    std::thread t1([&] { emit("A"); });
    std::thread t2([&] { emit("B"); });
    t1.join();
    t2.join();
    assert(messages.load() == 2);
    assert(oss.str().size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section05/std_print_unicode_thread_safety_cpp23", run>;

}  // namespace
