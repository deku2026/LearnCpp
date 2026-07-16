// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : yield_value
// Topic id : part2/stage12/section02/yield_value
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/yield_value";

class YieldTask {
public:
    struct promise_type {
        int current{};
        int yields{};
        YieldTask get_return_object() noexcept {
            return YieldTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        std::suspend_always yield_value(int value) noexcept {
            current = value;
            ++yields;
            return {};
        }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit YieldTask(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~YieldTask() {
        if (handle_) handle_.destroy();
    }
    bool next() {
        if (handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }
    int value() const { return handle_.promise().current; }
    int yield_count() const { return handle_.promise().yields; }

private:
    std::coroutine_handle<promise_type> handle_;
};

YieldTask values() {
    co_yield 3;
    co_yield 5;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto task = values();
    std::vector<int> observed;
    while (task.next()) observed.push_back(task.value());
    LEARN_EXPECT_EQ(checks, observed, std::vector<int>({3, 5}));
    LEARN_EXPECT_EQ(checks, task.yield_count(), 2);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/yield_value", run>;

}  // namespace
