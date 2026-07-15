// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 9.1 验收点
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : data_race_is_ub_recap
// Topic id : part2/stage11/section03/data_race_is_ub_recap
// Refs     : https://en.cppreference.com/w/cpp/language/memory_model
//            https://clang.llvm.org/docs/ThreadSanitizer.html
//            ISO [intro.races]
//
// Data race = concurrent access to same location, ≥1 write, no HB → UB.
// This demo shows the WRONG pattern only under a compile-time guard so
// default runs stay race-free; the FIXED patterns always run.
// Linux TSan: clang++ -std=c++23 -fsanitize=thread -g race.cpp -pthread

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [data race = UB] definition ===\n";
    std::cout << "  Two threads access the same memory location concurrently,\n"
                 "  at least one is a write, and there is no happens-before\n"
                 "  between them → undefined behavior (not \"maybe stale\").\n";

    std::cout << "=== FIXED: atomic counter ===\n";
    {
        std::atomic<int> counter{0};
        {
            std::jthread t1([&] {
                for (int i = 0; i < 10000; ++i) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
            std::jthread t2([&] {
                for (int i = 0; i < 10000; ++i) {
                    counter.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        assert(counter.load() == 20000);
        std::cout << "  atomic counter=" << counter.load() << '\n';
    }

    std::cout << "=== FIXED: mutex-protected counter ===\n";
    {
        std::mutex mtx;
        int counter = 0;
        auto bump = [&] {
            for (int i = 0; i < 10000; ++i) {
                std::lock_guard lock(mtx);
                ++counter;
            }
        };
        {
            std::jthread t1(bump);
            std::jthread t2(bump);
        }
        assert(counter == 20000);
        std::cout << "  mutex counter=" << counter << '\n';
    }

#if defined(LEARNCPP_DEMONSTRATE_DATA_RACE)
    // Intentionally racy — enable only for TSan education:
    //   clang++ -DLEARNCPP_DEMONSTRATE_DATA_RACE -fsanitize=thread ...
    std::cout << "=== INTENTIONAL RACE (TSan should report) ===\n";
    {
        int counter = 0;
        auto unsafe = [&] {
            for (int i = 0; i < 100000; ++i) {
                ++counter;  // data race
            }
        };
        std::jthread t1(unsafe), t2(unsafe);
        std::cout << "  racy counter (UB)=" << counter << '\n';
    }
#else
    std::cout << "=== intentional race demo compiled out ===\n";
    std::cout << "  define LEARNCPP_DEMONSTRATE_DATA_RACE + build with TSan\n"
                 "  on Linux/Clang-GCC CI to see ThreadSanitizer warnings.\n";
#endif

    std::cout << "eliminate races with atomic OR mutex — not optional polish\n";
    std::cout << "[data_race_is_ub_recap] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/data_race_is_ub_recap", run>;

}  // namespace
