// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : atomic_ref_cpp20
// Topic id : part2/stage11/section03/atomic_ref_cpp20
//
// Covers: std::atomic_ref (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_atomic_ref) && __cpp_lib_atomic_ref >= 201806L
    int x = 0;
    {
        std::atomic_ref<int> ar(x);
        ar.store(10);
        LEARN_CHECK(ar.load() == 10);
    }
    LEARN_CHECK(x == 10);
#else
    int x = 10;
    LEARN_CHECK(x == 10);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_atomic_ref) && __cpp_lib_atomic_ref >= 201806L
    int counter = 0;
    auto bump = [&] {
        std::atomic_ref<int> ar(counter);
        for (int i = 0; i < 500; ++i) {
            ar.fetch_add(1);
        }
    };
    std::thread t1(bump);
    std::thread t2(bump);
    t1.join();
    t2.join();
    LEARN_CHECK(counter == 1000);
#else
    std::atomic<int> counter{0};
    auto bump = [&] {
        for (int i = 0; i < 500; ++i) {
            counter.fetch_add(1);
        }
    };
    std::thread t1(bump);
    std::thread t2(bump);
    t1.join();
    t2.join();
    LEARN_CHECK(counter.load() == 1000);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_atomic_ref) && __cpp_lib_atomic_ref >= 201806L
    alignas(std::atomic_ref<int>::required_alignment) int x = 1;
    std::atomic_ref<int> ar(x);
    int expected = 1;
    bool ok = ar.compare_exchange_strong(expected, 9);
    LEARN_CHECK(ok);
    LEARN_CHECK(x == 9);
#else
    std::atomic<int> x{1};
    int expected = 1;
    bool ok = x.compare_exchange_strong(expected, 9);
    LEARN_CHECK(ok);
    LEARN_CHECK(x.load() == 9);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/atomic_ref_cpp20", run>;

}  // namespace
