// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_recursive_mutex
// Topic id : part2/stage11/section02/std_recursive_mutex
//

#include "learn/example_support.hpp"

#include <mutex>
#include <thread>

namespace {

void add_depth(std::recursive_mutex& mutex, const int depth, int& total) {
    std::lock_guard lock{mutex};
    total += depth;
    if (depth > 0) {
        add_depth(mutex, depth - 1, total);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/std_recursive_mutex"};
    std::recursive_mutex mutex;
    int total = 0;
    std::thread worker{add_depth, std::ref(mutex), 3, std::ref(total)};
    worker.join();

    LEARN_EXPECT_EQ(checks, total, 6);

#if 0
    // Re-entering the same non-recursive mutex from one thread would deadlock.
    std::mutex ordinary;
    std::lock_guard first{ordinary};
    std::lock_guard second{ordinary};
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_recursive_mutex", run>;

}  // namespace
