// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : jthread_stop_token
// Topic id : part2/stage11/section01/jthread_stop_token
//

#include "learn/example_support.hpp"

#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    learn::ExampleChecks checks{"part2/stage11/section01/jthread_stop_token"};
    std::mutex mutex;
    std::condition_variable_any condition;
    bool work_available = false;
    bool stopped_in_wait = false;
    std::promise<void> entered;
    auto entered_future = entered.get_future();

    std::jthread worker([&](const std::stop_token token) {
        entered.set_value();
        std::unique_lock lock{mutex};
        const bool predicate_won = condition.wait(lock, token, [&work_available] { return work_available; });
        stopped_in_wait = !predicate_won && token.stop_requested();
    });

    entered_future.get();
    const bool first_request = worker.request_stop();
    const bool second_request = worker.request_stop();
    worker.join();

    LEARN_EXPECT(checks, first_request);
    LEARN_EXPECT(checks, !second_request);
    LEARN_EXPECT(checks, stopped_in_wait);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage11/section01/jthread_stop_token",
                                             "jthread stop-token integration");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/jthread_stop_token", run>;

}  // namespace
