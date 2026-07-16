// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : std_suspend_always_and_never
// Topic id : part2/stage12/section03/std_suspend_always_and_never

#include "learn/example_support.hpp"

#include <array>
#include <coroutine>
#include <exception>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section03/std_suspend_always_and_never";

struct Trace {
    std::array<std::string_view, 8> events{};
    std::size_t size{};
    void push(std::string_view event) noexcept { events[size++] = event; }
};

class Demonstration {
public:
    struct promise_type {
        std::exception_ptr exception;
        Demonstration get_return_object() noexcept {
            return Demonstration{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit Demonstration(Handle handle) noexcept : handle_(handle) {}
    ~Demonstration() {
        if (handle_) {
            handle_.destroy();
        }
    }
    Demonstration(const Demonstration&) = delete;
    Demonstration& operator=(const Demonstration&) = delete;
    Demonstration(Demonstration&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Demonstration& operator=(Demonstration&&) = delete;

    [[nodiscard]] bool done() const noexcept { return handle_.done(); }
    void resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    Handle handle_{};
};

Demonstration compare(Trace& trace) {
    trace.push("before-never");
    co_await std::suspend_never{};
    trace.push("after-never");
    co_await std::suspend_always{};
    trace.push("after-always");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    static_assert(std::suspend_never{}.await_ready());
    static_assert(!std::suspend_always{}.await_ready());

    Trace trace;
    auto task = compare(trace);
    LEARN_EXPECT(checks, !task.done());
    LEARN_EXPECT_EQ(checks, trace.size, 2U);
    LEARN_EXPECT_EQ(checks, trace.events[0], "before-never");
    LEARN_EXPECT_EQ(checks, trace.events[1], "after-never");
    task.resume();
    LEARN_EXPECT(checks, task.done());
    LEARN_EXPECT_EQ(checks, trace.events[2], "after-always");
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/std_suspend_always_and_never", run>;

}  // namespace
