// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_thread
// Topic id : part2/stage11/section01/std_thread
//

#include "learn/example_support.hpp"

#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section01/std_thread"};
    int answer = 0;
    std::thread worker([&answer] { answer = 6 * 7; });
    LEARN_EXPECT(checks, worker.joinable());
    worker.join();
    LEARN_EXPECT(checks, !worker.joinable());
    LEARN_EXPECT_EQ(checks, answer, 42);

    int moved_result = 0;
    std::thread original([&moved_result] { moved_result = 7; });
    std::thread owner = std::move(original);
    LEARN_EXPECT(checks, !original.joinable());
    LEARN_EXPECT(checks, owner.joinable());
    owner.join();
    LEARN_EXPECT_EQ(checks, moved_result, 7);

#if 0
    std::thread forgotten([] {});
    // Destroying a joinable std::thread calls std::terminate; join or detach deliberately.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_thread", run>;

}  // namespace
