// LearnCpp topic example
// Doc      : part2-stage11-concurrency-and-threading.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_counting_semaphore_cpp20
// Topic id : part2/stage11/section02/std_counting_semaphore_cpp20
//
// Covers: std::counting_semaphore (C++20)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <semaphore>
#include <thread>
#include <vector>

namespace {

void demo_basics() {
#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    std::counting_semaphore<2> sem{1};
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
    std::counting_semaphore<> sem{0};
    std::atomic<int> got{0};
    std::thread t([&] {
        sem.acquire();
        got.store(1);
    });
    sem.release();
    t.join();
    LEARN_CHECK(got.load() == 1);
#else
    std::atomic<int> got{0};
    std::thread t([&] { got.store(1); });
    t.join();
    LEARN_CHECK(got.load() == 1);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    std::counting_semaphore<3> pool{3};
    std::atomic<int> concurrent{0};
    std::atomic<int> max_seen{0};
    std::vector<std::thread> ts;
    for (int i = 0; i < 6; ++i) {
        ts.emplace_back([&] {
            pool.acquire();
            int c = concurrent.fetch_add(1) + 1;
            int prev = max_seen.load();
            while (c > prev && !max_seen.compare_exchange_weak(prev, c)) {
            }
            concurrent.fetch_sub(1);
            pool.release();
        });
    }
    for (auto& t : ts) {
        t.join();
    }
    LEARN_CHECK(max_seen.load() <= 3);
#else
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_counting_semaphore_cpp20", run>;

}  // namespace
