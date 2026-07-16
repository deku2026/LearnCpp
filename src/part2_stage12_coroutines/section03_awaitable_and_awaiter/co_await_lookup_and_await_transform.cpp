// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : co_await_lookup_and_await_transform
// Topic id : part2/stage12/section03/co_await_lookup_and_await_transform

#include "learn/example_support.hpp"

#include <array>
#include <coroutine>
#include <exception>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section03/co_await_lookup_and_await_transform";

struct Trace {
    std::array<std::string_view, 8> events{};
    std::size_t size{};
    void push(std::string_view event) noexcept { events[size++] = event; }
};

struct RawToken {
    int value;
    Trace* trace;
};

struct TokenAwaiter {
    int value;
    Trace* trace;

    bool await_ready() const noexcept {
        trace->push("await_ready");
        return true;
    }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const noexcept {
        trace->push("await_resume");
        return value;
    }
};

struct WrappedToken {
    int value;
    Trace* trace;

    TokenAwaiter operator co_await() const noexcept {
        trace->push("operator-co-await");
        return TokenAwaiter{value, trace};
    }
};

class LookupTask {
public:
    struct promise_type {
        std::exception_ptr exception;

        LookupTask get_return_object() noexcept {
            return LookupTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        WrappedToken await_transform(RawToken token) const noexcept {
            token.trace->push("await_transform");
            return WrappedToken{token.value * 2, token.trace};
        }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit LookupTask(Handle handle) noexcept : handle_(handle) {}
    ~LookupTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    LookupTask(const LookupTask&) = delete;
    LookupTask& operator=(const LookupTask&) = delete;
    LookupTask(LookupTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    LookupTask& operator=(LookupTask&&) = delete;

    void rethrow_if_exception() const {
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    Handle handle_{};
};

LookupTask lookup(Trace& trace) {
    const int transformed = co_await RawToken{21, &trace};
    if (transformed == 42) {
        trace.push("body-after");
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    Trace trace;
    const auto task = lookup(trace);
    task.rethrow_if_exception();
    LEARN_EXPECT_EQ(checks, trace.size, 5U);
    LEARN_EXPECT_EQ(checks, trace.events[0], "await_transform");
    LEARN_EXPECT_EQ(checks, trace.events[1], "operator-co-await");
    LEARN_EXPECT_EQ(checks, trace.events[2], "await_ready");
    LEARN_EXPECT_EQ(checks, trace.events[3], "await_resume");
    LEARN_EXPECT_EQ(checks, trace.events[4], "body-after");
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/co_await_lookup_and_await_transform", run>;

}  // namespace
