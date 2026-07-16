// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : final_suspend
// Topic id : part2/stage12/section02/final_suspend
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/final_suspend";

class FinalTask {
public:
    struct promise_type {
        inline static int alive{};
        promise_type() { ++alive; }
        ~promise_type() { --alive; }
        FinalTask get_return_object() noexcept {
            return FinalTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit FinalTask(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~FinalTask() {
        if (handle_) handle_.destroy();
    }
    void resume() const { handle_.resume(); }
    bool done() const { return handle_.done(); }

private:
    std::coroutine_handle<promise_type> handle_;
};

FinalTask complete() {
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, FinalTask::promise_type::alive, 0);
    {
        auto task = complete();
        LEARN_EXPECT_EQ(checks, FinalTask::promise_type::alive, 1);
        task.resume();
        LEARN_EXPECT(checks, task.done());
        LEARN_EXPECT_EQ(checks, FinalTask::promise_type::alive, 1);  // final_suspend keeps the frame for its owner.
    }
    LEARN_EXPECT_EQ(checks, FinalTask::promise_type::alive, 0);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/final_suspend", run>;

}  // namespace
