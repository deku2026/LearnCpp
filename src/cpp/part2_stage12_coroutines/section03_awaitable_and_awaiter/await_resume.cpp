// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : await_resume
// Topic id : part2/stage12/section03/await_resume
//
// 要点: 协程恢复后调用 await_resume(); 其返回值 = 整个 co_await 表达式的值;
//       也可抛异常(传播给 co_await 点)。void 返回 → co_await 作语句用。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   [expr.await]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

struct ValueAwaiter {
    int v;
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const { return v * 2; }  // co_await 表达式结果
};

struct VoidAwaiter {
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    void await_resume() const noexcept {}  // 仅副作用 / 同步点
};

struct ThrowingAwaiter {
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const { throw std::runtime_error("await_resume_failed"); }
};

template <typename T>
struct ResTask {
    struct promise_type {
        T result{};
        std::exception_ptr ep{};
        ResTask get_return_object() { return ResTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T v) { result = std::move(v); }
        void unhandled_exception() { ep = std::current_exception(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit ResTask(Handle h) noexcept : h_(h) {}
    ~ResTask() {
        if (h_) {
            h_.destroy();
        }
    }
    ResTask(ResTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ResTask(const ResTask&) = delete;

    T run() {
        h_.resume();
        if (h_.promise().ep) {
            std::rethrow_exception(h_.promise().ep);
        }
        return std::move(h_.promise().result);
    }

private:
    Handle h_{};
};

struct VoidOnly {
    struct promise_type {
        std::exception_ptr ep{};
        VoidOnly get_return_object() { return VoidOnly{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { ep = std::current_exception(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit VoidOnly(Handle h) noexcept : h_(h) {}
    ~VoidOnly() {
        if (h_) {
            h_.destroy();
        }
    }
    VoidOnly(VoidOnly&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    VoidOnly(const VoidOnly&) = delete;

    void run() {
        h_.resume();
        if (h_.promise().ep) {
            std::rethrow_exception(h_.promise().ep);
        }
        assert(h_.done());
    }

private:
    Handle h_{};
};

ResTask<int> scale() {
    int x = co_await ValueAwaiter{21};  // → 42
    co_return x;
}

VoidOnly barrier() {
    co_await VoidAwaiter{};  // 语句形式
    co_return;
}

ResTask<int> fails() {
    int x = co_await ThrowingAwaiter{};
    co_return x;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== await_resume ===\n";

    {
        auto t = scale();
        assert(t.run() == 42);
        std::cout << "  await_resume returns value → co_await result 42\n";
    }
    {
        auto t = barrier();
        t.run();
        std::cout << "  await_resume void → statement co_await\n";
    }
    {
        auto t = fails();
        [[maybe_unused]] bool caught = false;
        try {
            (void)t.run();
        } catch (const std::runtime_error& e) {
            caught = true;
            assert(std::string(e.what()) == "await_resume_failed");
            std::cout << "  await_resume throw → " << e.what() << '\n';
        }
        assert(caught);
    }

    std::cout << "await_resume: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/await_resume", run>;

}  // namespace
