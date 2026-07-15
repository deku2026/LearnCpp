// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : compare_exchange_cas
// Topic id : part2/stage11/section03/compare_exchange_cas
//
// Covers: CAS strong/weak loops

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <thread>

namespace {

void demo_basics() {
    std::atomic<int> x{1};
    int expected = 1;
    bool ok = x.compare_exchange_strong(expected, 2);
    assert(ok);
    assert(x.load() == 2);
    expected = 1;
    ok = x.compare_exchange_strong(expected, 3);
    assert(!ok);
    assert(expected == 2);
}

void demo_intermediate() {
    std::atomic<int> x{0};
    auto cas_add = [&](int delta) {
        int cur = x.load();
        while (!x.compare_exchange_weak(cur, cur + delta)) {
        }
    };
    std::thread t1([&] {
        for (int i = 0; i < 200; ++i) {
            cas_add(1);
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 200; ++i) {
            cas_add(1);
        }
    });
    t1.join();
    t2.join();
    assert(x.load() == 400);
}

void demo_expert() {
    std::atomic<int> flag{0};
    int expected = 0;
    bool first = flag.compare_exchange_strong(expected, 1);
    assert(first);
    expected = 0;
    bool second = flag.compare_exchange_strong(expected, 1);
    assert(!second);
    assert(expected == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/compare_exchange_cas", run>;

}  // namespace
