// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : unhandled_exception
// Topic id : part2/stage12/section02/unhandled_exception
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <stdexcept>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/unhandled_exception";

class FailingTask {
public:
    struct promise_type {
        std::exception_ptr failure;
        FailingTask get_return_object() noexcept {
            return FailingTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { failure = std::current_exception(); }
    };
    explicit FailingTask(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~FailingTask() {
        if (handle_) handle_.destroy();
    }
    void run() {
        handle_.resume();
        if (handle_.promise().failure) std::rethrow_exception(handle_.promise().failure);
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

FailingTask fail() {
    throw std::runtime_error{"inside coroutine"};
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto task = fail();
    LEARN_EXPECT_THROWS(checks, std::runtime_error, task.run());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/unhandled_exception", run>;

}  // namespace
