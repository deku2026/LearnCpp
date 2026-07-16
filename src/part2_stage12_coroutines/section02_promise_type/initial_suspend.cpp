// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : initial_suspend
// Topic id : part2/stage12/section02/initial_suspend
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/initial_suspend";

template <bool Eager>
class StartPolicy {
public:
    struct promise_type {
        StartPolicy get_return_object() noexcept {
            return StartPolicy{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        auto initial_suspend() const noexcept {
            if constexpr (Eager)
                return std::suspend_never{};
            else
                return std::suspend_always{};
        }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit StartPolicy(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~StartPolicy() {
        if (handle_) handle_.destroy();
    }
    void resume() const {
        if (!handle_.done()) handle_.resume();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

template <bool Eager>
StartPolicy<Eager> set_flag(bool& flag) {
    flag = true;
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    bool eager_started = false;
    bool lazy_started = false;
    auto eager = set_flag<true>(eager_started);
    auto lazy = set_flag<false>(lazy_started);
    LEARN_EXPECT(checks, eager_started);
    LEARN_EXPECT(checks, !lazy_started);
    lazy.resume();
    LEARN_EXPECT(checks, lazy_started);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/initial_suspend", run>;

}  // namespace
