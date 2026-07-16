// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_async_future_promise
// Topic id : part2/stage11/section01/std_async_future_promise
//

#include "learn/example_support.hpp"

#include <future>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section01/std_async_future_promise"};
    auto asynchronous = std::async(std::launch::async, [](const int value) { return value * value; }, 9);
    LEARN_EXPECT(checks, asynchronous.valid());
    LEARN_EXPECT_EQ(checks, asynchronous.get(), 81);
    LEARN_EXPECT(checks, !asynchronous.valid());

    std::promise<int> promise;
    auto promised_value = promise.get_future();
    std::thread producer([promise = std::move(promise)]() mutable { promise.set_value(42); });
    LEARN_EXPECT_EQ(checks, promised_value.get(), 42);
    producer.join();

    std::promise<int> exceptional_promise;
    auto exceptional_future = exceptional_promise.get_future();
    exceptional_promise.set_exception(std::make_exception_ptr(std::runtime_error{"task failed"}));
    LEARN_EXPECT_THROWS(checks, std::runtime_error, exceptional_future.get());
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_async_future_promise", run>;

}  // namespace
