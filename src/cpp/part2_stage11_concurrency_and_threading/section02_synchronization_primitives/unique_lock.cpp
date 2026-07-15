// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 4.2
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : unique_lock
// Topic id : part2/stage11/section02/unique_lock
// Refs     : https://en.cppreference.com/w/cpp/thread/unique_lock
//            ISO [thread.lock.unique]
//
// unique_lock: flexible RAII lock — defer/try/timed, manual unlock, movable.
// Required partner of condition_variable (must be unique_lock).

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

namespace {

using namespace std::chrono_literals;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [unique_lock] basic RAII ===\n";
    {
        std::mutex mtx;
        int x = 0;
        {
            std::unique_lock lock(mtx);
            ++x;
            assert(lock.owns_lock());
        }
        assert(x == 1);
    }

    std::cout << "=== defer_lock + manual lock/unlock ===\n";
    {
        std::mutex mtx;
        std::unique_lock lock(mtx, std::defer_lock);
        assert(!lock.owns_lock());
        lock.lock();
        assert(lock.owns_lock());
        // Early unlock shrinks the critical section before notify-style work.
        lock.unlock();
        assert(!lock.owns_lock());
        lock.lock();
        lock.unlock();
        std::cout << "  defer_lock then explicit lock/unlock ok\n";
    }

    std::cout << "=== try_to_lock / try_lock_for ===\n";
    {
        std::mutex mtx;
        std::unique_lock holder(mtx);  // holds for the whole block
        std::unique_lock tryer(mtx, std::try_to_lock);
        assert(!tryer.owns_lock());

        std::timed_mutex tmtx;
        std::unique_lock tholder(tmtx);
        std::unique_lock timed(tmtx, std::defer_lock);
        const bool got = timed.try_lock_for(5ms);
        assert(!got);
        tholder.unlock();
        assert(timed.try_lock_for(50ms));
        timed.unlock();
        std::cout << "  try_to_lock / try_lock_for behave as expected\n";
    }

    std::cout << "=== movable: transfer ownership ===\n";
    {
        std::mutex mtx;
        std::unique_lock a(mtx);
        std::unique_lock b = std::move(a);
        assert(!a.owns_lock());
        assert(b.owns_lock());
        b.unlock();
    }

    std::cout << "=== with condition_variable: must be unique_lock ===\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;

        std::jthread producer([&] {
            {
                std::lock_guard lock(mtx);
                ready = true;
            }
            cv.notify_one();
        });

        std::unique_lock lock(mtx);
        cv.wait(lock, [&] { return ready; });  // may unlock while waiting
        assert(ready);
        std::cout << "  wait with predicate succeeded\n";
    }

    std::cout << "[unique_lock] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/unique_lock", run>;

}  // namespace
