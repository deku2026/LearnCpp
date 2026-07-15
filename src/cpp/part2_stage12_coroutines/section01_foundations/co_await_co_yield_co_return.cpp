// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : co_await_co_yield_co_return
// Topic id : part2/stage12/section01/co_await_co_yield_co_return
//
// 要点: 三关键字任一即可成协程:
//   co_yield v  ≈ co_await promise.yield_value(v)
//   co_return v → promise.return_value(v)  (或 return_void)
//   co_await e  → awaiter 三函数驱动挂起/恢复
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   [expr.await] [expr.yield] [stmt.return.coroutine]

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <optional>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// 惰性 Generator: 演示 co_yield + 隐式 co_return(落到末尾 → return_void)
// ---------------------------------------------------------------------------
template <typename T>
struct YieldGen {
    struct promise_type {
        T current{};
        YieldGen get_return_object() { return YieldGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit YieldGen(Handle h) noexcept : h_(h) {}
    ~YieldGen() {
        if (h_) {
            h_.destroy();
        }
    }
    YieldGen(YieldGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    YieldGen(const YieldGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().current; }

private:
    Handle h_{};
};

YieldGen<int> squares(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i* i;  // co_yield
    }
    // 落到末尾 = 隐式 co_return; → return_void()
}

// ---------------------------------------------------------------------------
// 最小 Task: 演示 co_await + co_return 值
// ---------------------------------------------------------------------------
template <typename T>
struct MiniTask {
    struct promise_type {
        std::optional<T> result;
        std::coroutine_handle<> cont{};

        MiniTask get_return_object() { return MiniTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                auto c = h.promise().cont;
                return c ? c : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(T v) { result = std::move(v); }
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit MiniTask(Handle h) noexcept : h_(h) {}
    ~MiniTask() {
        if (h_) {
            h_.destroy();
        }
    }
    MiniTask(MiniTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    MiniTask(const MiniTask&) = delete;

    // 同步跑到完成(顶层驱动, 教学用)
    T sync_get() {
        h_.resume();
        assert(h_.done());
        return std::move(*h_.promise().result);
    }

    auto operator co_await() {
        struct Awaiter {
            Handle callee;
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
                callee.promise().cont = caller;
                return callee;  // 对称转移
            }
            T await_resume() { return std::move(*callee.promise().result); }
        };
        return Awaiter{h_};
    }

private:
    Handle h_{};
};

// 已就绪 awaitable: 演示纯 co_await 路径
struct Immediate {
    int v;
    bool await_ready() const noexcept { return true; }  // 不挂起
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const noexcept { return v; }
};

MiniTask<int> add_via_await(int x) {
    int y = co_await Immediate{x + 1};  // co_await
    co_return y * 2;                    // co_return 值
}

MiniTask<int> chain() {
    int z = co_await add_via_await(20);  // co_await 另一个 Task
    co_return z;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== co_await_co_yield_co_return ===\n";

    auto g = squares(3);
    assert(g.next() && g.value() == 1);
    assert(g.next() && g.value() == 4);
    assert(g.next() && g.value() == 9);
    assert(!g.next());
    std::cout << "  co_yield path: 1 4 9\n";

    auto t = chain();
    const int r = t.sync_get();
    // add_via_await(20): await 21 → return 42
    assert(r == 42);
    std::cout << "  co_await + co_return path: " << r << '\n';

    std::cout << "co_await_co_yield_co_return: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section01/co_await_co_yield_co_return", run>;

}  // namespace
