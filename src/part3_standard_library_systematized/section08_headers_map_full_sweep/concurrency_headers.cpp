// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : concurrency_headers
// Topic id : part3/section08/concurrency_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <future>
#include <mutex>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part3/section08/concurrency_headers";

constexpr std::array<std::string_view, 11> headers{
    "atomic",    "barrier",      "condition_variable", "future",     "latch",  "mutex",
    "semaphore", "shared_mutex", "stop_token",         "syncstream", "thread",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::promise<int> promise;
    auto future = promise.get_future();
    std::jthread worker{[promise = std::move(promise)]() mutable { promise.set_value(23); }};
    LEARN_EXPECT_EQ(checks, future.get(), 23);
    worker.join();
    std::atomic<int> value{1};
    value.fetch_add(2, std::memory_order_relaxed);
    LEARN_EXPECT_EQ(checks, value.load(std::memory_order_relaxed), 3);
    LEARN_EXPECT_EQ(checks, headers.size(), 11U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/concurrency_headers", run>;

}  // namespace
