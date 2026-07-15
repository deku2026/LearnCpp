// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 3.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_packaged_task
// Topic id : part2/stage11/section01/std_packaged_task
// Refs     : https://en.cppreference.com/w/cpp/thread/packaged_task
//            ISO [futures.task]
//
// packaged_task: wrap a callable so its return/exception lands in a future.
// Typical use: hand the task to a worker / thread pool.

#include "learn/topic_registry.hpp"

#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace {

int add(int a, int b) {
    return a + b;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [packaged_task] move task onto a thread ===\n";
    {
        std::packaged_task<int(int, int)> task(add);
        std::future<int> fut = task.get_future();

        // packaged_task is move-only; transfer ownership to the thread.
        std::jthread t(std::move(task), 3, 4);

        assert(fut.get() == 7);
        std::cout << "  3+4=" << 7 << '\n';
    }

    std::cout << "=== invoke packaged_task on the current thread ===\n";
    {
        std::packaged_task<int()> task([] { return 100; });
        std::future<int> fut = task.get_future();
        task();  // run here
        assert(fut.get() == 100);
    }

    std::cout << "=== exceptions flow into the future ===\n";
    {
        std::packaged_task<void()> task([] { throw std::runtime_error("task failed"); });
        std::future<void> fut = task.get_future();
        std::jthread t(std::move(task));

        [[maybe_unused]] bool caught = false;
        try {
            fut.get();
        } catch (const std::runtime_error& e) {
            caught = true;
            std::cout << "  " << e.what() << '\n';
        }
        assert(caught);
    }

    std::cout << "=== reset for reuse of the same callable wrapper ===\n";
    {
        std::packaged_task<int(int)> task([](int x) { return x * 2; });
        auto f1 = task.get_future();
        task(21);
        assert(f1.get() == 42);

        task.reset();  // new shared state; same callable
        auto f2 = task.get_future();
        task(5);
        assert(f2.get() == 10);
        std::cout << "  reset then 5*2=10\n";
    }

    std::cout << "choice guide: async=simple one-shot; "
                 "promise=manual set; packaged_task=pool-friendly wrapper; "
                 "jthread=long-lived lifecycle\n";
    std::cout << "[std_packaged_task] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_packaged_task", run>;

}  // namespace
