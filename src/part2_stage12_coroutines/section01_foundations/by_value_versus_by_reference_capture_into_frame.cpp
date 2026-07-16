// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : by_value_versus_by_reference_capture_into_frame
// Topic id : part2/stage12/section01/by_value_versus_by_reference_capture_into_frame
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section01/by_value_versus_by_reference_capture_into_frame";

class SizeTask {
public:
    struct promise_type {
        std::size_t result{};
        SizeTask get_return_object() noexcept {
            return SizeTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_value(std::size_t value) noexcept { result = value; }
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit SizeTask(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    SizeTask(SizeTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    ~SizeTask() {
        if (handle_) handle_.destroy();
    }
    std::size_t finish() {
        handle_.resume();
        return handle_.promise().result;
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

SizeTask own_parameter(std::string text) {
    co_return text.size();
}
SizeTask borrow_parameter(const std::string& text) {
    co_return text.size();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::string source = "abc";
    auto owned = own_parameter(source);
    auto borrowed = borrow_parameter(source);
    source += "def";
    LEARN_EXPECT_EQ(checks, owned.finish(), 3U);     // The by-value parameter lives in the frame.
    LEARN_EXPECT_EQ(checks, borrowed.finish(), 6U);  // The reference still names the live caller object.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage12/section01/by_value_versus_by_reference_capture_into_frame", run>;

}  // namespace
