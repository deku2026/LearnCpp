// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section06_hand_rolled_examples
// Item     : co_await_chained_resumption
// Topic id : part2/stage12/section06/co_await_chained_resumption

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <optional>
#include <stdexcept>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section06/co_await_chained_resumption";

class ChainTask {
public:
    struct promise_type {
        std::optional<int> result;
        std::exception_ptr exception;
        std::coroutine_handle<> continuation{};

        ChainTask get_return_object() noexcept {
            return ChainTask{std::coroutine_handle<promise_type>::from_promise(*this)};
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
        void return_value(int value) noexcept { result = value; }
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit ChainTask(Handle handle) noexcept : handle_(handle) {}
    ~ChainTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    ChainTask(const ChainTask&) = delete;
    ChainTask& operator=(const ChainTask&) = delete;
    ChainTask(ChainTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    ChainTask& operator=(ChainTask&&) = delete;

    class Awaiter {
    public:
        explicit Awaiter(Handle handle) noexcept : handle_(handle) {}
        ~Awaiter() {
            if (handle_) {
                handle_.destroy();
            }
        }
        Awaiter(const Awaiter&) = delete;
        Awaiter& operator=(const Awaiter&) = delete;
        Awaiter(Awaiter&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
        Awaiter& operator=(Awaiter&&) = delete;

        bool await_ready() const noexcept { return handle_.done(); }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) const noexcept {
            handle_.promise().continuation = caller;
            return handle_;
        }
        int await_resume() const {
            if (handle_.promise().exception) {
                std::rethrow_exception(handle_.promise().exception);
            }
            return handle_.promise().result.value();
        }

    private:
        Handle handle_{};
    };

    Awaiter operator co_await() && noexcept { return Awaiter{std::exchange(handle_, {})}; }
    Awaiter operator co_await() & = delete;

    [[nodiscard]] int run() {
        handle_.resume();
        if (!handle_.done()) {
            throw std::logic_error{"unexpected external suspension"};
        }
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return handle_.promise().result.value();
    }

private:
    Handle handle_{};
};

struct ChainStats {
    int started{};
    int completed{};
};

ChainTask chain(int depth, ChainStats& stats) {
    ++stats.started;
    if (depth == 0) {
        ++stats.completed;
        co_return 0;
    }
    const int nested = co_await chain(depth - 1, stats);
    ++stats.completed;
    co_return nested + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    constexpr int depth = 512;
    ChainStats stats;
    auto task = chain(depth, stats);
    LEARN_EXPECT_EQ(checks, stats.started, 0);
    LEARN_EXPECT_EQ(checks, task.run(), depth);
    LEARN_EXPECT_EQ(checks, stats.started, depth + 1);
    LEARN_EXPECT_EQ(checks, stats.completed, depth + 1);
    // Every await_suspend/final_suspend returns a handle, so the chain uses symmetric transfer.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section06/co_await_chained_resumption", run>;

}  // namespace
