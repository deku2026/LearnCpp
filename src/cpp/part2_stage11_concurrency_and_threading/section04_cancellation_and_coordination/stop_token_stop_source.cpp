// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : stop_token_stop_source
// Topic id : part2/stage11/section04/stop_token_stop_source
//
// Covers: stop_source / stop_token / stop_callback

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <stop_token>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::stop_source src;
    std::stop_token tok = src.get_token();
    assert(tok.stop_possible());
    assert(!tok.stop_requested());
    src.request_stop();
    assert(tok.stop_requested());
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::stop_source src;
    std::atomic<int> hits{0};
    std::stop_callback cb(src.get_token(), [&] { hits.fetch_add(1); });
    src.request_stop();
    assert(hits.load() == 1);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    std::stop_source src;
    std::atomic<bool> exited{false};
    std::thread t([tok = src.get_token(), &exited] {
        while (!tok.stop_requested()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        exited.store(true);
    });
    src.request_stop();
    t.join();
    assert(exited.load());
#else
    std::atomic<bool> stop{false};
    std::atomic<bool> exited{false};
    std::thread t([&] {
        while (!stop.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        exited.store(true);
    });
    stop.store(true);
    t.join();
    assert(exited.load());
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section04/stop_token_stop_source", run>;

}  // namespace
