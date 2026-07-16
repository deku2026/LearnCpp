// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : await_ready
// Topic id : part2/stage12/section03/await_ready

#include "learn/example_support.hpp"

#include <array>
#include <coroutine>
#include <exception>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section03/await_ready";

struct Trace {
    std::array<std::string_view, 8> events{};
    std::size_t size{};

    void push(std::string_view event) noexcept { events[size++] = event; }
};

class TraceTask {
public:
    struct promise_type {
        std::exception_ptr exception;

        TraceTask get_return_object() noexcept {
            return TraceTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit TraceTask(Handle handle) noexcept : handle_(handle) {}
    ~TraceTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    TraceTask(const TraceTask&) = delete;
    TraceTask& operator=(const TraceTask&) = delete;
    TraceTask(TraceTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    TraceTask& operator=(TraceTask&&) = delete;

    [[nodiscard]] bool done() const noexcept { return !handle_ || handle_.done(); }

    void resume() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
        }
        if (handle_ && handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    Handle handle_{};
};

struct ProbeAwaiter {
    bool ready;
    Trace* trace;

    bool await_ready() const noexcept {
        trace->push("await_ready");
        return ready;
    }
    void await_suspend(std::coroutine_handle<>) const noexcept { trace->push("await_suspend"); }
    int await_resume() const noexcept {
        trace->push("await_resume");
        return 42;
    }
};

TraceTask observe(bool ready, Trace& trace) {
    trace.push("body-enter");
    const int value = co_await ProbeAwaiter{ready, &trace};
    if (value == 42) {
        trace.push("body-after");
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    Trace immediate_trace;
    auto immediate = observe(true, immediate_trace);
    LEARN_EXPECT(checks, immediate.done());
    LEARN_EXPECT_EQ(checks, immediate_trace.size, 4U);
    LEARN_EXPECT_EQ(checks, immediate_trace.events[0], "body-enter");
    LEARN_EXPECT_EQ(checks, immediate_trace.events[1], "await_ready");
    LEARN_EXPECT_EQ(checks, immediate_trace.events[2], "await_resume");
    LEARN_EXPECT_EQ(checks, immediate_trace.events[3], "body-after");

    Trace suspended_trace;
    auto suspended = observe(false, suspended_trace);
    LEARN_EXPECT(checks, !suspended.done());
    LEARN_EXPECT_EQ(checks, suspended_trace.size, 3U);
    LEARN_EXPECT_EQ(checks, suspended_trace.events[2], "await_suspend");
    suspended.resume();
    LEARN_EXPECT(checks, suspended.done());
    LEARN_EXPECT_EQ(checks, suspended_trace.events[3], "await_resume");
    LEARN_EXPECT_EQ(checks, suspended_trace.events[4], "body-after");
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/await_ready", run>;

}  // namespace
