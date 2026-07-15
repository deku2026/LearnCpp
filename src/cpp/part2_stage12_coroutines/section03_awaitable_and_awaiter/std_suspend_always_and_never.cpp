// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : std_suspend_always_and_never
// Topic id : part2/stage12/section03/std_suspend_always_and_never
//
// 要点: <coroutine> 两个平凡 awaiter —
//   suspend_always: await_ready=false → 总是挂起
//   suspend_never:  await_ready=true  → 从不挂起
//   理解它们 = 理解 initial/final/yield 返回它们的含义。
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/suspend_always
//   https://en.cppreference.com/w/cpp/coroutine/suspend_never

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <utility>

namespace {

// 手写等价物对照
struct MyAlways {
    constexpr bool await_ready() const noexcept { return false; }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    constexpr void await_resume() const noexcept {}
};
struct MyNever {
    constexpr bool await_ready() const noexcept { return true; }
    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    constexpr void await_resume() const noexcept {}
};

static_assert(std::suspend_always{}.await_ready() == false);
static_assert(std::suspend_never{}.await_ready() == true);
static_assert(MyAlways{}.await_ready() == false);
static_assert(MyNever{}.await_ready() == true);

int g_body = 0;

struct Toggle {
    struct promise_type {
        int value = 0;
        Toggle get_return_object() { return Toggle{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        // 用标准 suspend_always 做懒启动
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int v) {
            value = v;
            return {};  // yield 点挂起
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Toggle(Handle h) noexcept : h_(h) {}
    ~Toggle() {
        if (h_) {
            h_.destroy();
        }
    }
    Toggle(Toggle&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Toggle(const Toggle&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    int value() const { return h_.promise().value; }

private:
    Handle h_{};
};

// 急启动: initial = suspend_never; body 内可直接 co_await 标准 awaiter
struct Eager {
    struct promise_type {
        Eager get_return_object() { return Eager{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Eager(Handle h) noexcept : h_(h) {}
    ~Eager() {
        if (h_) {
            h_.destroy();
        }
    }
    Eager(Eager&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Eager(const Eager&) = delete;

    void resume() { h_.resume(); }
    bool done() const noexcept { return h_.done(); }

private:
    Handle h_{};
};

Toggle lazy_yield() {
    g_body = 1;
    co_yield 100;
    co_yield 200;
}

Eager eager_clean() {
    g_body = 10;
    co_await std::suspend_never{};   // 明示: 不挂起, 继续
    co_await std::suspend_always{};  // 手动让出点
    g_body = 11;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== std_suspend_always_and_never ===\n";

    g_body = 0;
    {
        auto g = lazy_yield();
        assert(g_body == 0);  // initial suspend_always
        assert(g.next() && g.value() == 100);
        assert(g_body == 1);
        assert(g.next() && g.value() == 200);
        assert(!g.next());
        std::cout << "  suspend_always @ initial/yield/final\n";
    }

    g_body = 0;
    {
        auto e = eager_clean();
        // initial suspend_never → 已执行到中途 suspend_always
        assert(g_body == 10);
        assert(!e.done());
        e.resume();
        assert(g_body == 11);
        assert(e.done());
        std::cout << "  suspend_never initial + suspend_always mid-point\n";
    }

    // 也可在算法里直接: co_await std::suspend_always{} 作手动让出点。
    std::cout << "std_suspend_always_and_never: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/std_suspend_always_and_never", run>;

}  // namespace
