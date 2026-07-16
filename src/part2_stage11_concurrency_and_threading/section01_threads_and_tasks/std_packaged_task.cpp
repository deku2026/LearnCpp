// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_packaged_task
// Topic id : part2/stage11/section01/std_packaged_task
//

#include "learn/example_support.hpp"

#include <future>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section01/std_packaged_task"};
    std::packaged_task<int(int, int)> task([](const int left, const int right) { return left * right; });
    auto result = task.get_future();
    LEARN_EXPECT(checks, task.valid());

    std::thread worker{std::move(task), 6, 7};
    LEARN_EXPECT(checks, !task.valid());
    LEARN_EXPECT_EQ(checks, result.get(), 42);
    worker.join();
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_packaged_task", run>;

}  // namespace
