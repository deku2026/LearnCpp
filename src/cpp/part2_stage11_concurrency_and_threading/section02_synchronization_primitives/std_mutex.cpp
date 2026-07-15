// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 4.1
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_mutex
// Topic id : part2/stage11/section02/std_mutex
// Refs     : https://en.cppreference.com/w/cpp/thread/mutex
//            ISO [thread.mutex.class]
//
// mutex: exclusive ownership of a critical section.
// Never call lock/unlock by hand in production paths — use RAII guards
// (next topics). This file shows the raw API once, then the safe pattern.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [std_mutex] exclusive critical section ===\n";
    {
        std::mutex mtx;
        int shared = 0;

        auto unsafe_looking_but_locked = [&] {
            // Teaching only: prefer lock_guard / unique_lock / scoped_lock.
            mtx.lock();
            ++shared;
            mtx.unlock();
        };

        {
            std::vector<std::jthread> pool;
            for (int i = 0; i < 8; ++i) {
                pool.emplace_back(unsafe_looking_but_locked);
            }
        }
        assert(shared == 8);
        std::cout << "  shared=" << shared << " (no lost updates)\n";
    }

    std::cout << "=== try_lock: non-blocking attempt ===\n";
    {
        std::mutex mtx;
        mtx.lock();
        [[maybe_unused]] const bool got = mtx.try_lock();  // fails: already owned by this thread
        assert(!got);
        mtx.unlock();
        assert(mtx.try_lock());
        mtx.unlock();
        std::cout << "  try_lock distinguishes free vs busy\n";
    }

    std::cout << "=== unlock is release, lock is acquire ===\n";
    std::cout << "  All writes in a critical section become visible to the "
                 "next locker (mutex = release-acquire pair). "
                 "Data race is UB; mutex establishes happens-before.\n";

    std::cout << "=== expert notes ===\n";
    // - Non-recursive: same thread must not lock() again → deadlock/UB depending on impl.
    // - Prefer lock_guard (simple), unique_lock (defer/adopt/transfer), scoped_lock (multi).
    // - Never unlock a mutex you do not own; never forget unlock on early return (RAII).
    std::cout << "  non-recursive; use RAII guards; see recursive_mutex / scoped_lock topics\n";

    std::cout << "[std_mutex] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_mutex", run>;

}  // namespace
