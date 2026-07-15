// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_binary_semaphore_cpp20
// Topic id : part2/stage11/section02/std_binary_semaphore_cpp20
//
// Covers: std::binary_semaphore (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <semaphore>
#include <thread>

namespace {

void demo_basics() {
#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    std::binary_semaphore sem{1};
    sem.acquire();
    LEARN_CHECK(!sem.try_acquire());
    sem.release();
    LEARN_CHECK(sem.try_acquire());
    sem.release();
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    std::binary_semaphore gate{0};
    std::atomic<int> order{0};
    std::thread t([&] {
        gate.acquire();
        order.store(2);
    });
    order.store(1);
    gate.release();
    t.join();
    LEARN_CHECK(order.load() == 2);
#else
    std::atomic<int> order{0};
    std::thread t([&] { order.store(2); });
    order.store(1);
    t.join();
    LEARN_CHECK(order.load() == 2);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    std::binary_semaphore sem{1};
    std::atomic<int> critical{0};
    auto work = [&] {
        for (int i = 0; i < 50; ++i) {
            sem.acquire();
            int v = critical.load();
            critical.store(v + 1);
            sem.release();
        }
    };
    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();
    LEARN_CHECK(critical.load() == 100);
#else
    std::atomic<int> critical{0};
    auto work = [&] {
        for (int i = 0; i < 50; ++i) {
            critical.fetch_add(1);
        }
    };
    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();
    LEARN_CHECK(critical.load() == 100);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_binary_semaphore_cpp20", run>;

}  // namespace
