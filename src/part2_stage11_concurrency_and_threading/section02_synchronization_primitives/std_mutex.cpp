// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_mutex
// Topic id : part2/stage11/section02/std_mutex
//

#include "learn/example_support.hpp"

#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/std_mutex"};
    std::mutex mutex;
    int counter = 0;
    std::vector<std::thread> workers;
    for (int worker_index = 0; worker_index < 3; ++worker_index) {
        workers.emplace_back([&mutex, &counter] {
            for (int iteration = 0; iteration < 500; ++iteration) {
                mutex.lock();
                ++counter;
                mutex.unlock();
            }
        });
    }
    for (std::thread& worker : workers) {
        worker.join();
    }

    LEARN_EXPECT_EQ(checks, counter, 1'500);

#if 0
    // Concurrent plain increments without the mutex would be a data race and UB.
    ++counter;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_mutex", run>;

}  // namespace
