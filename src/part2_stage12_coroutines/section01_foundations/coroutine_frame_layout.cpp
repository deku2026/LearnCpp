// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : coroutine_frame_layout
// Topic id : part2/stage12/section01/coroutine_frame_layout
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section01/coroutine_frame_layout";

class FrameProbe {
public:
    struct promise_type {
        int result{};
        FrameProbe get_return_object() noexcept {
            return FrameProbe{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_value(int value) noexcept { result = value; }
        void unhandled_exception() const noexcept { std::terminate(); }
    };

    explicit FrameProbe(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~FrameProbe() {
        if (handle_) {
            handle_.destroy();
        }
    }
    void resume() { handle_.resume(); }
    [[nodiscard]] bool done() const { return handle_.done(); }
    [[nodiscard]] int result() const { return handle_.promise().result; }

private:
    std::coroutine_handle<promise_type> handle_;
};

FrameProbe inspect_local(const int*& suspended_address) {
    int local = 7;
    suspended_address = &local;
    co_await std::suspend_always{};
    co_return local + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const int* address = nullptr;
    auto probe = inspect_local(address);
    probe.resume();
    LEARN_EXPECT(checks, address != nullptr);
    LEARN_EXPECT_EQ(checks, *address, 7);  // The local survives while execution is suspended.
    probe.resume();
    LEARN_EXPECT(checks, probe.done());
    LEARN_EXPECT_EQ(checks, probe.result(), 8);
    // Do not dereference address after the local's lifetime ends; the wrapper later destroys the frame.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section01/coroutine_frame_layout", run>;

}  // namespace
