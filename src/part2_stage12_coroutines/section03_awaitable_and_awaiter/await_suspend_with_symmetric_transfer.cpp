// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : await_suspend_with_symmetric_transfer
// Topic id : part2/stage12/section03/await_suspend_with_symmetric_transfer

#include "learn/example_support.hpp"

#include <array>
#include <coroutine>
#include <exception>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section03/await_suspend_with_symmetric_transfer";

struct Trace {
    std::array<std::string_view, 8> events{};
    std::size_t size{};
    void push(std::string_view event) noexcept { events[size++] = event; }
};

class TransferTask {
public:
    struct promise_type {
        std::coroutine_handle<> continuation{};
        std::exception_ptr exception;

        TransferTask get_return_object() noexcept {
            return TransferTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> completed) const noexcept {
                const auto next_coroutine = completed.promise().continuation;
                return next_coroutine ? next_coroutine : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };

        FinalAwaiter final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit TransferTask(Handle handle) noexcept : handle_(handle) {}
    ~TransferTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    TransferTask(const TransferTask&) = delete;
    TransferTask& operator=(const TransferTask&) = delete;
    TransferTask(TransferTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    TransferTask& operator=(TransferTask&&) = delete;

    struct BorrowedAwaiter {
        Handle callee;

        bool await_ready() const noexcept { return !callee || callee.done(); }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) const noexcept {
            callee.promise().continuation = caller;
            return callee;  // Return, do not call resume(): this is symmetric transfer.
        }
        void await_resume() const {
            if (callee.promise().exception) {
                std::rethrow_exception(callee.promise().exception);
            }
        }
    };

    BorrowedAwaiter operator co_await() & noexcept { return BorrowedAwaiter{handle_}; }
    BorrowedAwaiter operator co_await() && = delete;

    void resume() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
        }
        rethrow_if_exception();
    }
    [[nodiscard]] bool done() const noexcept { return handle_ && handle_.done(); }
    void rethrow_if_exception() const {
        if (handle_ && handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    Handle handle_{};
};

TransferTask target(Trace& trace) {
    trace.push("target-enter");
    co_return;
}

TransferTask source(TransferTask& next, Trace& trace) {
    trace.push("source-before");
    co_await next;
    trace.push("source-after");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    Trace trace;
    auto callee = target(trace);  // Declared first, so it outlives the borrow in caller's frame.
    auto caller = source(callee, trace);
    caller.resume();

    LEARN_EXPECT(checks, caller.done());
    LEARN_EXPECT(checks, callee.done());
    LEARN_EXPECT_EQ(checks, trace.size, 3U);
    LEARN_EXPECT_EQ(checks, trace.events[0], "source-before");
    LEARN_EXPECT_EQ(checks, trace.events[1], "target-enter");
    LEARN_EXPECT_EQ(checks, trace.events[2], "source-after");
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/await_suspend_with_symmetric_transfer", run>;

}  // namespace
