// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 1.1
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_thread
// Topic id : part2/stage11/section01/std_thread
// Refs     : https://en.cppreference.com/w/cpp/thread/thread
//            ISO [thread.thread.class]
//
// std::thread (C++11): spawn a new OS thread running a callable.
// Fatal pitfall: if joinable at destructor time → std::terminate.
// Prefer jthread (next topic) for RAII join + cooperative cancel.

#include "learn/topic_registry.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace {

using namespace std::chrono_literals;

void worker(int id) {
    std::cout << "  worker " << id << " on thread " << std::this_thread::get_id() << '\n';
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [std_thread] launch + join ===\n";
    {
        std::thread t1(worker, 1);
        std::thread t2([] { std::cout << "  lambda thread " << std::this_thread::get_id() << '\n'; });

        // Arguments are decay-copied into thread-local storage of the new thread.
        // Use std::ref / std::cref only when you intentionally share an object.
        assert(t1.joinable());
        assert(t2.joinable());

        t1.join();  // block until finished
        t2.join();
        assert(!t1.joinable());
        assert(!t2.joinable());
    }

    std::cout << "=== move ownership (thread is move-only) ===\n";
    {
        std::thread t([] { std::this_thread::sleep_for(5ms); });
        std::thread u = std::move(t);
        assert(!t.joinable());
        assert(u.joinable());
        u.join();
    }

    std::cout << "=== this_thread helpers ===\n";
    {
        const auto id = std::this_thread::get_id();
        std::cout << "  main id=" << id << '\n';
        std::this_thread::yield();         // hint: reschedule
        std::this_thread::sleep_for(1ms);  // block for duration
    }

    std::cout << "=== N workers (always join every thread) ===\n";
    {
        std::vector<std::thread> pool;
        pool.reserve(4);
        for (int i = 0; i < 4; ++i) {
            pool.emplace_back(worker, i);
        }
        for (auto& t : pool) {
            t.join();
        }
    }

    // Do NOT detach here in teaching code unless lifetime of all shared
    // data is proven longer than the detached thread. Prefer join/jthread.
    std::cout << "[std_thread] all checks passed "
                 "(remember: join or detach before dtor, or terminate)\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_thread", run>;

}  // namespace
