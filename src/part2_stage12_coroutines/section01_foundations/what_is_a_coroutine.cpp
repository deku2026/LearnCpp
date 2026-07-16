// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : what_is_a_coroutine
// Topic id : part2/stage12/section01/what_is_a_coroutine
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section01/what_is_a_coroutine";

class LazyAction {
public:
    struct promise_type {
        LazyAction get_return_object() noexcept {
            return LazyAction{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept { std::terminate(); }
    };

    explicit LazyAction(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    LazyAction(const LazyAction&) = delete;
    ~LazyAction() {
        if (handle_) {
            handle_.destroy();
        }
    }
    void resume() { handle_.resume(); }
    [[nodiscard]] bool done() const { return handle_.done(); }

private:
    std::coroutine_handle<promise_type> handle_;
};

LazyAction mark_started(bool& started) {
    started = true;
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    bool started = false;
    auto action = mark_started(started);  // Calling creates a coroutine frame and returns its handle wrapper.
    LEARN_EXPECT(checks, !started);
    action.resume();
    LEARN_EXPECT(checks, started);
    LEARN_EXPECT(checks, action.done());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section01/what_is_a_coroutine", run>;

}  // namespace
