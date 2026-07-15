// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 4.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : lock_guard
// Topic id : part2/stage11/section02/lock_guard
// Refs     : https://en.cppreference.com/w/cpp/thread/lock_guard
//            ISO [thread.lock.guard]
//
// lock_guard: simplest RAII mutex owner — lock in ctor, unlock in dtor.
// Non-movable; no manual unlock. Default choice for plain critical sections.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [lock_guard] RAII critical section ===\n";
    {
        std::mutex mtx;
        int counter = 0;

        auto bump = [&] {
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;
        };

        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < 10; ++i) {
                pool.emplace_back(bump);
            }
        }
        assert(counter == 10);
        std::cout << "  counter=" << counter << '\n';
    }

    std::cout << "=== unlock even when exception escapes ===\n";
    {
        std::mutex mtx;
        try {
            std::lock_guard lock(mtx);  // CTAD (C++17)
            throw std::runtime_error("boom");
        } catch (const std::exception& e) {
            std::cout << "  caught: " << e.what() << " (mutex already unlocked)\n";
        }
        // If unlock were manual and skipped, next lock would deadlock.
        {
            std::lock_guard lock(mtx);
            std::cout << "  re-acquired successfully after throw\n";
        }
    }

    std::cout << "=== adopt_lock: already locked externally ===\n";
    {
        std::mutex mtx;
        mtx.lock();
        {
            std::lock_guard lock(mtx, std::adopt_lock);
            // owns the already-locked mutex; will unlock on leave
        }
        assert(mtx.try_lock());
        mtx.unlock();
    }

    std::cout << "[lock_guard] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/lock_guard", run>;

}  // namespace
