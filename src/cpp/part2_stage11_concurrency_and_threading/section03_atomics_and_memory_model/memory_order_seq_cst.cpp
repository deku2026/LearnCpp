// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : memory_order_seq_cst
// Topic id : part2/stage11/section03/memory_order_seq_cst
//
// Covers: memory_order_seq_cst default

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.store(1);
    assert(x.load() == 1);
}

void demo_intermediate() {
    std::atomic<int> x{0};
    std::atomic<int> y{0};
    std::thread t1([&] {
        x.store(1, std::memory_order_seq_cst);
        (void)y.load(std::memory_order_seq_cst);
    });
    std::thread t2([&] {
        y.store(1, std::memory_order_seq_cst);
        (void)x.load(std::memory_order_seq_cst);
    });
    t1.join();
    t2.join();
    assert(x.load() == 1 && y.load() == 1);
}

void demo_expert() {
    std::atomic<int> flag{0};
    int payload = 0;
    std::thread p([&] {
        payload = 11;
        flag.store(1, std::memory_order_seq_cst);
    });
    std::thread c([&] {
        while (flag.load(std::memory_order_seq_cst) == 0) {
        }
        assert(payload == 11);
    });
    p.join();
    c.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/memory_order_seq_cst", run>;

}  // namespace
