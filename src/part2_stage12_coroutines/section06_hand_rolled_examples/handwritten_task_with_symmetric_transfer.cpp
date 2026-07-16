// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section06_hand_rolled_examples
// Item     : handwritten_task_with_symmetric_transfer
// Topic id : part2/stage12/section06/handwritten_task_with_symmetric_transfer

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section06/handwritten_task_with_symmetric_transfer";

template <class T>
class Task {
public:
    struct promise_type {
        std::optional<T> result;
        std::exception_ptr exception;
        std::coroutine_handle<> continuation{};

        Task get_return_object() noexcept { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
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
        void return_value(T value) noexcept(std::is_nothrow_move_constructible_v<T>) {
            result.emplace(std::move(value));
        }
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Task(Handle handle) noexcept : handle_(handle) {}
    ~Task() { reset(); }
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            reset();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

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

        bool await_ready() const noexcept { return !handle_ || handle_.done(); }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) const noexcept {
            handle_.promise().continuation = caller;
            return handle_;  // Symmetric transfer into the awaited task.
        }
        T await_resume() {
            if (handle_.promise().exception) {
                std::rethrow_exception(handle_.promise().exception);
            }
            return std::move(handle_.promise().result.value());
        }

    private:
        Handle handle_{};
    };

    Awaiter operator co_await() && noexcept { return Awaiter{std::exchange(handle_, {})}; }
    Awaiter operator co_await() & = delete;

    [[nodiscard]] T run() {
        if (!handle_.done()) {
            handle_.resume();
        }
        if (!handle_.done()) {
            throw std::logic_error{"task awaited an external scheduler"};
        }
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return std::move(handle_.promise().result.value());
    }

private:
    void reset() noexcept {
        if (handle_) {
            handle_.destroy();
            handle_ = {};
        }
    }

    Handle handle_{};
};

Task<int> leaf(std::vector<std::string>& trace) {
    trace.emplace_back("leaf");
    co_return 40;
}

Task<int> middle(std::vector<std::string>& trace) {
    trace.emplace_back("middle-before");
    const int value = co_await leaf(trace);
    trace.emplace_back("middle-after");
    co_return value + 2;
}

Task<int> root_task(std::vector<std::string>& trace) {
    trace.emplace_back("root-before");
    const int value = co_await middle(trace);
    trace.emplace_back("root-after");
    co_return value * 2;
}

Task<int> failing_leaf() {
    throw std::runtime_error{"inner task failed"};
    co_return 0;
}

Task<int> failing_root() {
    co_return co_await failing_leaf();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    std::vector<std::string> trace;
    auto task = root_task(trace);
    LEARN_EXPECT(checks, trace.empty());  // Lazy initial_suspend.
    LEARN_EXPECT_EQ(checks, task.run(), 84);
    LEARN_EXPECT_EQ(checks, (trace),
                    (std::vector<std::string>{"root-before", "middle-before", "leaf", "middle-after", "root-after"}));

    auto failed = failing_root();
    LEARN_EXPECT_THROWS(checks, std::runtime_error, failed.run());
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage12/section06/handwritten_task_with_symmetric_transfer", run>;

}  // namespace
