// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : what_is_a_coroutine
// Topic id : part2/stage12/section01/what_is_a_coroutine
//
// 要点: 函数体含 co_await / co_yield / co_return 任一 → 即为协程;
//       返回类型必须带 promise_type; stackless: 状态在堆上的协程帧。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   [dcl.fct.def.coroutine]  Lewis Baker: Coroutine Theory

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// 最小 Generator: 只为演示“什么是协程”, 完整版见 section06。
// ---------------------------------------------------------------------------
template <typename T>
struct MiniGen {
    struct promise_type {
        T current{};

        MiniGen get_return_object() { return MiniGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
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

    explicit MiniGen(Handle h) noexcept : h_(h) {}
    ~MiniGen() {
        if (h_) {
            h_.destroy();
        }
    }
    MiniGen(MiniGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    MiniGen(const MiniGen&) = delete;
    MiniGen& operator=(const MiniGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().current; }

private:
    Handle h_{};
};

// 有 co_yield → 这是协程(编译器改写为状态机 + 协程帧)。
MiniGen<int> counter(int from, int to) {
    for (int i = from; i < to; ++i) {
        co_yield i;  // 挂起点: 产一个值, 把控制权交回调用者
    }
}

// 普通函数: 一次跑完, 无挂起能力。
int sum_range(int from, int to) {
    int s = 0;
    for (int i = from; i < to; ++i) {
        s += i;
    }
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== what_is_a_coroutine ===\n";

    // --- 协程: 可挂起 / 恢复, 局部状态保存在协程帧里 ---
    auto g = counter(1, 5);
    [[maybe_unused]] int expected = 1;
    while (g.next()) {
        std::cout << "  yield " << g.value() << '\n';
        assert(g.value() == expected);
        ++expected;
    }
    assert(expected == 5);

    // --- 普通函数: 一次跑完 ---
    const int s = sum_range(1, 5);
    std::cout << "  ordinary sum_range(1,5) = " << s << '\n';
    assert(s == 10);

    // 对比心智:
    //   普通函数 → 用调用方栈, 返回即结束
    //   线程     → 独立栈 + OS 抢占
    //   C++ 协程 → stackless: 无独立栈, 状态在堆帧, 协作式挂起/恢复
    //
    // 🔶 Qt 对照: 协程把异步写成顺序代码; Qt 事件循环 + 信号槽是回调模型。
    // ⚠️ main / 构造 / 析构 / constexpr 函数不能是协程。

    std::cout << "what_is_a_coroutine: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section01/what_is_a_coroutine", run>;

}  // namespace
