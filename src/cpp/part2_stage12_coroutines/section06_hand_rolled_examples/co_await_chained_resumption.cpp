// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section06_hand_rolled_examples
// Item     : co_await_chained_resumption
// Topic id : part2/stage12/section06/co_await_chained_resumption
//
// 要点: 验收 — 一个 co_await 点前后帧/promise/awaiter 时序;
//       多级 co_await 链如何通过 continuation 对称转移恢复。
// Refs:
//   文档步骤 6 + 9–10
//   Lewis Baker: Understanding operator co_await / Symmetric Transfer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace {

std::vector<std::string> g_trace;

void note(const char* s) {
    g_trace.emplace_back(s);
}

template <typename T>
class Task {
public:
    struct promise_type {
        std::optional<T> result;
        std::coroutine_handle<> cont{};

        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                note("final_suspend→transfer cont");
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
    explicit Task(Handle h) noexcept : h_(h) {}
    ~Task() {
        if (h_) {
            h_.destroy();
        }
    }
    Task(Task&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Task(const Task&) = delete;

    T get() {
        note("top.resume");
        h_.resume();
        assert(h_.done());
        return std::move(*h_.promise().result);
    }

    auto operator co_await() {
        struct Awaiter {
            Handle callee;
            bool await_ready() const noexcept {
                note("awaiter.await_ready=false");
                return false;
            }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
                note("awaiter.await_suspend: save cont + transfer");
                // ③ 协程已挂起, 帧保存局部+挂起点
                // ④ await_suspend 安排后续: 挂 continuation, 尾调用 callee
                callee.promise().cont = caller;
                return callee;
            }
            T await_resume() {
                note("awaiter.await_resume");
                // ⑥ 恢复后取结果
                return std::move(*callee.promise().result);
            }
        };
        return Awaiter{h_};
    }

private:
    Handle h_{};
};

Task<int> level0() {
    note("level0.body");
    co_return 7;
}

Task<int> level1() {
    note("level1.before_await");
    // 【co_await 点】完整时序见文档 6.1
    int x = co_await level0();
    note("level1.after_await");
    co_return x + 1;
}

Task<int> level2() {
    note("level2.before_await");
    int y = co_await level1();
    note("level2.after_await");
    co_return y * 3;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== co_await_chained_resumption ===\n";
    g_trace.clear();

    auto t = level2();
    const int r = t.get();
    assert(r == 24);  // (7+1)*3

    std::cout << "  trace:\n";
    for (const auto& s : g_trace) {
        std::cout << "    " << s << '\n';
    }

    // 期望关键片段存在:
    // top.resume → level2 body → await_suspend transfer → level1 → level0
    // → final 恢复 → await_resume 沿链返回
    bool saw_suspend = false;
    bool saw_resume = false;
    for (const auto& s : g_trace) {
        if (s.find("await_suspend") != std::string::npos) {
            saw_suspend = true;
        }
        if (s.find("await_resume") != std::string::npos) {
            saw_resume = true;
        }
    }
    assert(saw_suspend && saw_resume);

    std::cout << "  result=" << r << "  (chain: L2 co_await L1 co_await L0, final transfers back)\n";
    std::cout << "co_await_chained_resumption: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section06/co_await_chained_resumption", run>;

}  // namespace
