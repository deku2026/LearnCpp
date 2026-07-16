// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : return_value_or_return_void
// Topic id : part2/stage12/section02/return_value_or_return_void
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/return_value_or_return_void";

template <class Promise>
class BasicTask {
public:
    using promise_type = Promise;
    explicit BasicTask(std::coroutine_handle<Promise> handle) : handle_(handle) {}
    ~BasicTask() {
        if (handle_) handle_.destroy();
    }
    void finish() const { handle_.resume(); }
    Promise& promise() const { return handle_.promise(); }

private:
    std::coroutine_handle<Promise> handle_;
};

struct ValuePromise {
    int value{};
    BasicTask<ValuePromise> get_return_object() noexcept {
        return BasicTask<ValuePromise>{std::coroutine_handle<ValuePromise>::from_promise(*this)};
    }
    std::suspend_always initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }
    void return_value(int result) noexcept { value = result; }
    void unhandled_exception() const noexcept { std::terminate(); }
};

struct VoidPromise {
    bool returned{};
    BasicTask<VoidPromise> get_return_object() noexcept {
        return BasicTask<VoidPromise>{std::coroutine_handle<VoidPromise>::from_promise(*this)};
    }
    std::suspend_always initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }
    void return_void() noexcept { returned = true; }
    void unhandled_exception() const noexcept { std::terminate(); }
};

BasicTask<ValuePromise> answer() {
    co_return 42;
}
BasicTask<VoidPromise> signal() {
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto value = answer();
    auto nothing = signal();
    value.finish();
    nothing.finish();
    LEARN_EXPECT_EQ(checks, value.promise().value, 42);
    LEARN_EXPECT(checks, nothing.promise().returned);
    // A promise type declares return_value or return_void, never both.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/return_value_or_return_void", run>;

}  // namespace
