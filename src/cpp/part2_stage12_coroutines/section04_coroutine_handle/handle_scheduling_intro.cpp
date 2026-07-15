// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section04_coroutine_handle
// Item     : handle_scheduling_intro
// Topic id : part2/stage12/section04/handle_scheduling_intro
//
// 要点: handle 只是“操控把手”; 谁/何时/在哪个线程 resume = 调度问题。
//   Generator: 迭代器 operator++ 同步 resume
//   Task:     await_suspend 挂 continuation, 由执行器/完成回调 resume
//   异步 IO:  await_suspend 把 handle 交给 IO, 完成线程 resume
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/coroutine_handle
//   Boost.Asio awaitable: 把 resume 投递到 executor(教学对照)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <deque>
#include <functional>
#include <iostream>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// 微型“调度器”: 队列里塞 handle, run() 逐个 resume
// ---------------------------------------------------------------------------
struct MiniScheduler {
    std::deque<std::coroutine_handle<>> q;

    void post(std::coroutine_handle<> h) { q.push_back(h); }

    void run() {
        while (!q.empty()) {
            auto h = q.front();
            q.pop_front();
            if (h && !h.done()) {
                h.resume();
            }
        }
    }
};

MiniScheduler* g_sched = nullptr;

struct ScheduleAwaiter {
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) const {
        // 把“恢复我”投递到调度器 —— 模拟 asio post 到 executor
        assert(g_sched);
        g_sched->post(h);
    }
    void await_resume() const noexcept {}
};

struct SchedTask {
    struct promise_type {
        int result = 0;
        SchedTask get_return_object() { return SchedTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { result = v; }
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit SchedTask(Handle h) noexcept : h_(h) {}
    ~SchedTask() {
        if (h_) {
            h_.destroy();
        }
    }
    SchedTask(SchedTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    SchedTask(const SchedTask&) = delete;

    Handle handle() const noexcept { return h_; }
    int result() const { return h_.promise().result; }
    bool done() const noexcept { return h_.done(); }

private:
    Handle h_{};
};

// 迭代驱动(Generator 风格) —— 对照
struct IterGen {
    struct promise_type {
        int v = 0;
        IterGen get_return_object() { return IterGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int x) {
            v = x;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit IterGen(Handle h) noexcept : h_(h) {}
    ~IterGen() {
        if (h_) {
            h_.destroy();
        }
    }
    IterGen(IterGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    IterGen(const IterGen&) = delete;

    struct iterator {
        Handle h{};
        void operator++() { h.resume(); }
        int operator*() const { return h.promise().v; }
        bool operator==(std::default_sentinel_t) const { return !h || h.done(); }
    };
    iterator begin() {
        if (h_) {
            h_.resume();
        }
        return iterator{h_};
    }
    std::default_sentinel_t end() const noexcept { return {}; }

private:
    Handle h_{};
};

SchedTask scheduled_sum() {
    int s = 0;
    co_await ScheduleAwaiter{};  // 让出, 稍后由调度器 resume
    s += 10;
    co_await ScheduleAwaiter{};
    s += 20;
    co_return s;
}

IterGen range3() {
    for (int i = 1; i <= 3; ++i) {
        co_yield i;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== handle_scheduling_intro ===\n";

    // A) Generator: 迭代器就是调度器
    {
        std::cout << "  iterator-driven:";
        [[maybe_unused]] int sum = 0;
        for (int x : range3()) {
            std::cout << ' ' << x;
            sum += x;
        }
        std::cout << '\n';
        assert(sum == 6);
    }

    // B) 显式调度队列驱动 Task
    {
        MiniScheduler sched;
        g_sched = &sched;
        auto t = scheduled_sum();
        // 启动: 首次 resume 跑到第一个 ScheduleAwaiter
        sched.post(t.handle());
        sched.run();
        assert(t.done());
        assert(t.result() == 30);
        g_sched = nullptr;
        std::cout << "  scheduler-driven task result=" << t.result() << '\n';
    }

    // 语言只给挂起/恢复机制; 标准目前只把“最简单调度”(同步迭代)
    // 做成了 std::generator。异步调度仍属库/C++26 execution 范畴。
    //
    // 🔶 Boost.Asio: co_spawn + awaitable 把 handle 与 io_context 绑定,
    //    完成处理程序里 executor.defer/post 再 resume —— 与 MiniScheduler 同构。

    std::cout << "handle_scheduling_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section04/handle_scheduling_intro", run>;

}  // namespace
