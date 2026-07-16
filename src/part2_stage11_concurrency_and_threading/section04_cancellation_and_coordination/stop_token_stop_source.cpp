// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section04_cancellation_and_coordination
// Item     : stop_token_stop_source
// Topic id : part2/stage11/section04/stop_token_stop_source
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <stop_token>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section04/stop_token_stop_source";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::stop_source source;
    const std::stop_token token = source.get_token();
    std::atomic<int> callbacks{};
    std::stop_callback callback{token, [&callbacks] { callbacks.fetch_add(1); }};

    LEARN_EXPECT(checks, token.stop_possible());
    LEARN_EXPECT(checks, !token.stop_requested());
    LEARN_EXPECT(checks, source.request_stop());
    LEARN_EXPECT(checks, token.stop_requested());
    LEARN_EXPECT_EQ(checks, callbacks.load(), 1);
    LEARN_EXPECT(checks, !source.request_stop());  // idempotent after first request
    LEARN_EXPECT_EQ(checks, callbacks.load(), 1);

    std::atomic<int> late_callbacks{};
    std::stop_callback late{token, [&late_callbacks] { ++late_callbacks; }};
    LEARN_EXPECT_EQ(checks, late_callbacks.load(), 1);  // invoked immediately
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section04/stop_token_stop_source", run>;

}  // namespace
