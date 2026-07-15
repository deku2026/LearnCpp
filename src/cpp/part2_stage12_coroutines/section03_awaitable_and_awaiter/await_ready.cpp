// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : await_ready
// Topic id : part2/stage12/section03/await_ready
//
// 要点: await_ready()==true → 跳过挂起, 直接 await_resume(快路径);
//       false → 挂起并走 await_suspend。用于“结果已就绪别白挂起”。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   Lewis Baker: Understanding operator co_await

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

std::vector<std::string> g_trace;

struct ReadyAware {
    int value;
    bool ready;
    const char* tag;

    bool await_ready() const noexcept {
        g_trace.push_back(std::string(tag) + ".await_ready=" + (ready ? "true" : "false"));
        return ready;
    }
    void await_suspend(std::coroutine_handle<>) const { g_trace.push_back(std::string(tag) + ".await_suspend"); }
    int await_resume() const {
        g_trace.push_back(std::string(tag) + ".await_resume");
        return value;
    }
};

struct ReadyTask {
    struct promise_type {
        int result = 0;
        ReadyTask get_return_object() { return ReadyTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { result = v; }
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit ReadyTask(Handle h) noexcept : h_(h) {}
    ~ReadyTask() {
        if (h_) {
            h_.destroy();
        }
    }
    ReadyTask(ReadyTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ReadyTask(const ReadyTask&) = delete;

    int run() {
        h_.resume();
        assert(h_.done());
        return h_.promise().result;
    }

private:
    Handle h_{};
};

ReadyTask demo() {
    // 已就绪: 不挂起
    int a = co_await ReadyAware{10, true, "hot"};
    // 未就绪: 会挂起 —— 但本 Task 无外部 resume 续体, 故用 “假挂起又立刻
    // 不可”; 这里改用 ready=true 路径为主, 另开一个会真正挂起的对照需外部驱动。
    // 为保持单文件可跑完: 第二下也 ready。
    int b = co_await ReadyAware{a + 1, true, "hot2"};
    co_return b;
}

// 可被外部 resume 的“手动挂起”协程, 展示 ready=false 路径
struct Manual {
    struct promise_type {
        Manual get_return_object() { return Manual{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Manual(Handle h) noexcept : h_(h) {}
    ~Manual() {
        if (h_) {
            h_.destroy();
        }
    }
    Manual(Manual&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Manual(const Manual&) = delete;

    Handle handle() const noexcept { return h_; }
    void resume() { h_.resume(); }
    bool done() const noexcept { return h_.done(); }

private:
    Handle h_{};
};

// await_suspend 把 handle 存到外部, 由 run() 再 resume
std::coroutine_handle<> g_parked{};

struct Park {
    bool await_ready() const noexcept {
        g_trace.push_back("park.await_ready=false");
        return false;
    }
    void await_suspend(std::coroutine_handle<> h) const {
        g_trace.push_back("park.await_suspend");
        g_parked = h;
    }
    void await_resume() const { g_trace.push_back("park.await_resume"); }
};

Manual park_once() {
    co_await Park{};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== await_ready ===\n";
    g_trace.clear();

    {
        auto t = demo();
        assert(t.run() == 11);
        // ready 路径: ready → resume, 无 suspend
        assert(g_trace[0] == "hot.await_ready=true");
        assert(g_trace[1] == "hot.await_resume");
        assert(g_trace[2] == "hot2.await_ready=true");
        assert(g_trace[3] == "hot2.await_resume");
        std::cout << "  ready=true path skips await_suspend\n";
    }

    g_trace.clear();
    g_parked = {};
    {
        auto m = park_once();
        m.resume();  // 跑到 park, ready=false → suspend
        assert(!m.done());
        assert(g_parked == m.handle());
        g_parked.resume();  // 外部恢复
        assert(m.done());
        assert(g_trace[0] == "park.await_ready=false");
        assert(g_trace[1] == "park.await_suspend");
        assert(g_trace[2] == "park.await_resume");
        std::cout << "  ready=false path: suspend then external resume\n";
    }

    std::cout << "await_ready: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/await_ready", run>;

}  // namespace
