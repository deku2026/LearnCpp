// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : lock_guard
// Topic id : part2/stage11/section02/lock_guard
//

#include "learn/example_support.hpp"

#include <mutex>
#include <stdexcept>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/lock_guard"};
    std::mutex mutex;
    int protected_value = 0;

    try {
        std::lock_guard lock{mutex};
        protected_value = 1;
        throw std::runtime_error{"demonstrate RAII unlock"};
    } catch (const std::runtime_error&) {
    }

    const bool unlocked_after_exception = mutex.try_lock();
    if (unlocked_after_exception) {
        mutex.unlock();
    }
    std::thread worker([&] {
        std::lock_guard lock{mutex};
        ++protected_value;
    });
    worker.join();

    LEARN_EXPECT(checks, unlocked_after_exception);
    LEARN_EXPECT_EQ(checks, protected_value, 2);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/lock_guard", run>;

}  // namespace
