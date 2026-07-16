// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_condition_variable
// Topic id : part2/stage11/section02/std_condition_variable
//

#include "learn/example_support.hpp"

#include <condition_variable>
#include <mutex>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/std_condition_variable"};
    std::mutex mutex;
    std::condition_variable condition;
    bool ready = false;
    int payload = 0;
    int observed = 0;

    std::thread consumer([&] {
        std::unique_lock lock{mutex};
        condition.wait(lock, [&ready] { return ready; });
        observed = payload;
    });

    {
        std::lock_guard lock{mutex};
        payload = 42;
        ready = true;
    }
    condition.notify_one();
    consumer.join();

    LEARN_EXPECT_EQ(checks, observed, 42);

#if 0
    // A bare wait is incorrect: notifications are not stored and wakeups may be spurious.
    std::unique_lock lock{mutex};
    condition.wait(lock);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_condition_variable", run>;

}  // namespace
