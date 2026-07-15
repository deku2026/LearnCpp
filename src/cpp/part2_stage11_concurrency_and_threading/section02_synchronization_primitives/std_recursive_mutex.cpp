// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 4.1
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_recursive_mutex
// Topic id : part2/stage11/section02/std_recursive_mutex
// Refs     : https://en.cppreference.com/w/cpp/thread/recursive_mutex
//            ISO [thread.mutex.recursive]
//
// recursive_mutex: same thread may lock multiple times; must unlock same count.
// Prefer redesign (flatten call graph) over recursion when possible — easier
// to reason about and slightly cheaper with plain mutex.

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <thread>

namespace {

class NestedCounter {
    std::recursive_mutex mtx_;
    int value_ = 0;

public:
    void outer() {
        std::lock_guard lock(mtx_);
        ++value_;
        inner();  // re-enters the same mutex — would deadlock with std::mutex
    }

    void inner() {
        std::lock_guard lock(mtx_);
        ++value_;
    }

    int get() {
        std::lock_guard lock(mtx_);
        return value_;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [recursive_mutex] re-entrant same-thread lock ===\n";
    {
        NestedCounter c;
        c.outer();
        assert(c.get() == 2);
        std::cout << "  outer+inner => value=2\n";
    }

    std::cout << "=== multi-thread still exclusive across threads ===\n";
    {
        std::recursive_mutex mtx;
        int shared = 0;

        auto work = [&] {
            for (int i = 0; i < 100; ++i) {
                std::lock_guard lock(mtx);
                {
                    std::lock_guard nested(mtx);  // same thread OK
                    ++shared;
                }
            }
        };

        {
            std::jthread t1(work);
            std::jthread t2(work);
        }
        assert(shared == 200);
        std::cout << "  shared=" << shared << '\n';
    }

    std::cout << "=== unlock count must match lock count ===\n";
    {
        std::recursive_mutex mtx;
        mtx.lock();
        mtx.lock();    // ownership depth = 2
        mtx.unlock();  // depth = 1, still owned by this thread
        mtx.unlock();  // depth = 0, free
        assert(mtx.try_lock());
        mtx.unlock();
        std::cout << "  balanced unlock restored free state\n";
    }

    std::cout << "prefer plain mutex + non-recursive API design when feasible\n";
    std::cout << "[std_recursive_mutex] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_recursive_mutex", run>;

}  // namespace
