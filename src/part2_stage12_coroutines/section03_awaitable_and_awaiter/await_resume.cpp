// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : await_resume
// Topic id : part2/stage12/section03/await_resume

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section03/await_resume";

template <class T>
class EagerValueTask {
public:
    struct promise_type {
        std::optional<T> value;
        std::exception_ptr exception;

        EagerValueTask get_return_object() noexcept {
            return EagerValueTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_value(T result) noexcept(std::is_nothrow_move_constructible_v<T>) {
            value.emplace(std::move(result));
        }
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit EagerValueTask(Handle handle) noexcept : handle_(handle) {}
    ~EagerValueTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    EagerValueTask(const EagerValueTask&) = delete;
    EagerValueTask& operator=(const EagerValueTask&) = delete;
    EagerValueTask(EagerValueTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    EagerValueTask& operator=(EagerValueTask&&) = delete;

    [[nodiscard]] const T& result() const {
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return handle_.promise().value.value();
    }

private:
    Handle handle_{};
};

struct CheckedValueAwaiter {
    int value;
    bool valid;
    int* suspend_calls;

    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept { ++*suspend_calls; }
    int await_resume() const {
        if (!valid) {
            throw std::domain_error{"awaited value rejected"};
        }
        return value;
    }
};

EagerValueTask<int> calculate(bool valid, int& suspend_calls) {
    const int value = co_await CheckedValueAwaiter{41, valid, &suspend_calls};
    co_return value + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    int suspend_calls{};
    const auto good = calculate(true, suspend_calls);
    LEARN_EXPECT_EQ(checks, good.result(), 42);
    LEARN_EXPECT_EQ(checks, suspend_calls, 0);  // await_ready skipped await_suspend.

    const auto bad = calculate(false, suspend_calls);
    LEARN_EXPECT_THROWS(checks, std::domain_error, bad.result());
    LEARN_EXPECT_EQ(checks, suspend_calls, 0);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/await_resume", run>;

}  // namespace
