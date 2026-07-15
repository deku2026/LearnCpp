// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : initial_suspend
// Topic id : part2/stage12/section02/initial_suspend
//
// 要点: initial_suspend 在函数体前 co_await:
//   suspend_always → 懒启动(创建即挂, 首次 resume 才跑 body)
//   suspend_never  → 急启动(创建就跑到第一个真正挂起点)
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/suspend_always
//   https://en.cppreference.com/w/cpp/coroutine/suspend_never

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

namespace {

int g_side_effect = 0;

template <bool Lazy>
struct LaunchStyle {
    struct promise_type {
        int value = -1;

        LaunchStyle get_return_object() {
            return LaunchStyle{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        // 用 if constexpr 切换懒/急 —— 两个独立最小状态机的策略差只在这里。
        auto initial_suspend() noexcept {
            if constexpr (Lazy) {
                return std::suspend_always{};
            } else {
                return std::suspend_never{};
            }
        }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int v) {
            value = v;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit LaunchStyle(Handle h) noexcept : h_(h) {}
    ~LaunchStyle() {
        if (h_) {
            h_.destroy();
        }
    }
    LaunchStyle(LaunchStyle&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    LaunchStyle(const LaunchStyle&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    int current() const { return h_.promise().value; }
    bool done() const noexcept { return h_.done(); }

private:
    Handle h_{};
};

LaunchStyle<true> lazy_coro() {
    g_side_effect = 1;  // 只有 resume 后才执行
    co_yield 42;
}

LaunchStyle<false> eager_coro() {
    g_side_effect = 2;  // 创建时立即执行到 co_yield
    co_yield 99;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== initial_suspend ===\n";

    g_side_effect = 0;
    {
        auto lazy = lazy_coro();
        // 懒: 刚创建 body 未跑
        assert(g_side_effect == 0);
        assert(lazy.current() == -1);
        assert(lazy.next() && lazy.current() == 42);
        assert(g_side_effect == 1);
        std::cout << "  lazy: side_effect after first resume = " << g_side_effect << '\n';
    }

    g_side_effect = 0;
    {
        auto eager = eager_coro();
        // 急: 创建后已跑到第一个 co_yield, 值已写入 promise
        assert(g_side_effect == 2);
        assert(eager.current() == 99);
        assert(!eager.done());
        // 再 resume → 结束
        assert(!eager.next());
        std::cout << "  eager: side_effect at construction = " << g_side_effect << '\n';
    }

    // Generator 几乎总懒启动; Task 也常懒启动(先挂 continuation 再跑)。
    std::cout << "initial_suspend: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/initial_suspend", run>;

}  // namespace
