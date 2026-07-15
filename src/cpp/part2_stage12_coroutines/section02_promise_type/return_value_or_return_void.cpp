// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : return_value_or_return_void
// Topic id : part2/stage12/section02/return_value_or_return_void
//
// 要点: co_return; / 落到末尾 → return_void();
//       co_return expr → return_value(expr);
//       二者在同一 promise 中【二选一】(不能同时提供可匹配重载)。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   [stmt.return.coroutine]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// 只 return_void: 适合 Generator / 无结果 Task
// ---------------------------------------------------------------------------
struct VoidTask {
    struct promise_type {
        bool finished = false;
        VoidTask get_return_object() { return VoidTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept { finished = true; }
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit VoidTask(Handle h) noexcept : h_(h) {}
    ~VoidTask() {
        if (h_) {
            h_.destroy();
        }
    }
    VoidTask(VoidTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    VoidTask(const VoidTask&) = delete;

    void run() {
        h_.resume();
        assert(h_.done());
    }
    bool finished() const { return h_.promise().finished; }

private:
    Handle h_{};
};

// ---------------------------------------------------------------------------
// 只 return_value: 适合产生单结果的 Task
// ---------------------------------------------------------------------------
template <typename T>
struct ValueTask {
    struct promise_type {
        std::optional<T> result;
        ValueTask get_return_object() { return ValueTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T v) { result = std::move(v); }
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit ValueTask(Handle h) noexcept : h_(h) {}
    ~ValueTask() {
        if (h_) {
            h_.destroy();
        }
    }
    ValueTask(ValueTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ValueTask(const ValueTask&) = delete;

    T run() {
        h_.resume();
        assert(h_.done());
        return std::move(*h_.promise().result);
    }

private:
    Handle h_{};
};

VoidTask just_void() {
    // 显式 co_return;
    co_return;
}

VoidTask fallthrough_void() {
    // 无 co_return: 落到末尾也调 return_void
    if (false) {
        co_return;
    }
}

ValueTask<int> answer() {
    co_return 42;
}

ValueTask<std::string> greeting() {
    std::string s = "hello";
    co_return s;  // move into return_value
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== return_value_or_return_void ===\n";

    {
        auto t = just_void();
        t.run();
        assert(t.finished());
        std::cout << "  co_return; → return_void\n";
    }
    {
        auto t = fallthrough_void();
        t.run();
        assert(t.finished());
        std::cout << "  fall-off-end → return_void\n";
    }
    {
        auto t = answer();
        assert(t.run() == 42);
        std::cout << "  co_return 42 → return_value\n";
    }
    {
        auto t = greeting();
        assert(t.run() == "hello");
        std::cout << "  co_return string → return_value\n";
    }

    // 若 promise 同时有 return_void 与 return_value, 且两者都可选用,
    // 程序非良构 —— 设计时按“有结果 / 无结果”二选一。

    std::cout << "return_value_or_return_void: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/return_value_or_return_void", run>;

}  // namespace
