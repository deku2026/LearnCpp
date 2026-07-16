// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : promise_type_intro
// Topic id : part2/stage12/section02/promise_type_intro
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/promise_type_intro";

class PromiseView {
public:
    struct promise_type {
        int returned{};
        PromiseView get_return_object() noexcept {
            return PromiseView{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_value(int value) noexcept { returned = value; }
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit PromiseView(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~PromiseView() {
        if (handle_) handle_.destroy();
    }
    promise_type& promise() const { return handle_.promise(); }
    void resume() const { handle_.resume(); }

private:
    std::coroutine_handle<promise_type> handle_;
};

PromiseView compute() {
    co_return 42;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto task = compute();
    auto* promise_address = &task.promise();
    task.resume();
    LEARN_EXPECT(checks, promise_address == &task.promise());
    LEARN_EXPECT_EQ(checks, task.promise().returned, 42);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/promise_type_intro", run>;

}  // namespace
