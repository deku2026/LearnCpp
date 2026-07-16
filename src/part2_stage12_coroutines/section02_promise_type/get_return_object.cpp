// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : get_return_object
// Topic id : part2/stage12/section02/get_return_object
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section02/get_return_object";

class ReturnObject {
public:
    struct promise_type {
        bool get_called{};
        ReturnObject get_return_object() noexcept {
            get_called = true;
            return ReturnObject{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit ReturnObject(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~ReturnObject() {
        if (handle_) handle_.destroy();
    }
    [[nodiscard]] bool created_by_promise() const { return handle_.promise().get_called; }
    void finish() const { handle_.resume(); }

private:
    std::coroutine_handle<promise_type> handle_;
};

ReturnObject make_return_object() {
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto object = make_return_object();
    LEARN_EXPECT(checks, object.created_by_promise());
    object.finish();
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section02/get_return_object", run>;

}  // namespace
