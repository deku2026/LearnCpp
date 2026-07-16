// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : thread_local_storage
// Topic id : part2/stage11/section01/thread_local_storage
//

#include "learn/example_support.hpp"

#include <array>
#include <thread>

namespace {

thread_local int local_calls = 0;

[[nodiscard]] int next_local_call() noexcept {
    return ++local_calls;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section01/thread_local_storage"};
    std::array<int, 2> worker_results{};
    std::array<std::thread, 2> workers{
        std::thread{[&worker_results] {
            static_cast<void>(next_local_call());
            worker_results[0] = next_local_call();
        }},
        std::thread{[&worker_results] {
            static_cast<void>(next_local_call());
            worker_results[1] = next_local_call();
        }},
    };
    for (std::thread& worker : workers) {
        worker.join();
    }

    const int main_thread_first_call = next_local_call();
    LEARN_EXPECT_EQ(checks, worker_results, (std::array{2, 2}));
    LEARN_EXPECT_EQ(checks, main_thread_first_call, 1);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/thread_local_storage", run>;

}  // namespace
