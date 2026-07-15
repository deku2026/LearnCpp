// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_acquire_release
// Topic id : part2/stage11/section03/memory_order_acquire_release
//
// Covers: release/acquire publish

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> ready{0};
    int data = 0;
    std::thread t([&] {
        data = 7;
        ready.store(1, std::memory_order_release);
    });
    while (ready.load(std::memory_order_acquire) == 0) {
    }
    LEARN_CHECK(data == 7);
    t.join();
}

void demo_intermediate() {
    std::atomic<int> flag{0};
    int a = 0;
    int b = 0;
    std::thread prod([&] {
        a = 1;
        b = 2;
        flag.store(1, std::memory_order_release);
    });
    std::thread cons([&] {
        while (flag.load(std::memory_order_acquire) == 0) {
        }
        LEARN_CHECK(a == 1 && b == 2);
    });
    prod.join();
    cons.join();
}

void demo_expert() {
    std::atomic<int> handoff{0};
    int payload = 0;
    std::thread writer([&] {
        payload = 99;
        handoff.store(1, std::memory_order_release);
    });
    std::thread reader([&] {
        int v = 0;
        while ((v = handoff.load(std::memory_order_acquire)) == 0) {
        }
        LEARN_CHECK(v == 1);
        LEARN_CHECK(payload == 99);
    });
    writer.join();
    reader.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_acquire_release", run>;

}  // namespace
