// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : co_await_co_yield_co_return
// Topic id : part2/stage12/section01/co_await_co_yield_co_return
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section01/co_await_co_yield_co_return";

class Tokens {
public:
    struct promise_type {
        int current{};
        Tokens get_return_object() noexcept { return Tokens{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        std::suspend_always yield_value(int value) noexcept {
            current = value;
            return {};
        }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept { std::terminate(); }
    };

    explicit Tokens(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~Tokens() {
        if (handle_) handle_.destroy();
    }
    bool next() {
        if (handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }
    int current() const { return handle_.promise().current; }

private:
    std::coroutine_handle<promise_type> handle_;
};

Tokens syntax_tour() {
    co_await std::suspend_never{};  // Awaiting a ready awaitable does not suspend.
    co_yield 10;
    co_yield 20;
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto tokens = syntax_tour();
    std::vector<int> values;
    while (tokens.next()) values.push_back(tokens.current());
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({10, 20}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section01/co_await_co_yield_co_return", run>;

}  // namespace
