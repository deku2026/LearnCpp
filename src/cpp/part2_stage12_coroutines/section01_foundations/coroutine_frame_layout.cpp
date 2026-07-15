// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : coroutine_frame_layout
// Topic id : part2/stage12/section01/coroutine_frame_layout
//
// 要点: 协程帧(堆分配, 默认可 HALO)含:
//   ① promise  ② 参数(按值拷/移进帧)  ③ 跨挂起局部变量  ④ 挂起点编号
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   P0981 HALO

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>

namespace {

// Trace 对象: 观察“拷进帧 / 跨挂起仍存活 / destroy 时析构”。
struct Trace {
    std::string tag;
    explicit Trace(std::string t) : tag(std::move(t)) { std::cout << "    Trace(" << tag << ") ctor\n"; }
    Trace(const Trace& o) : tag(o.tag + ".copy") { std::cout << "    Trace(" << tag << ") copy-ctor\n"; }
    Trace(Trace&& o) noexcept : tag(std::move(o.tag)) {
        tag += ".move";
        std::cout << "    Trace(" << tag << ") move-ctor\n";
    }
    ~Trace() { std::cout << "    Trace(" << tag << ") dtor\n"; }
    Trace& operator=(const Trace&) = delete;
    Trace& operator=(Trace&&) = delete;
};

template <typename T>
struct FrameDemo {
    struct promise_type {
        T current{};
        // 帧里还有: 参数副本、跨挂起局部、挂起点状态 —— 由编译器布局。

        FrameDemo get_return_object() { return FrameDemo{std::coroutine_handle<promise_type>::from_promise(*this)}; }
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
    explicit FrameDemo(Handle h) noexcept : h_(h) {}
    ~FrameDemo() {
        if (h_) {
            h_.destroy();  // 析构帧内局部 + promise, 释放堆帧
        }
    }
    FrameDemo(FrameDemo&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    FrameDemo(const FrameDemo&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().current; }
    void* frame_address() const noexcept { return h_.address(); }

private:
    Handle h_{};
};

// param 按值进帧; local 跨 co_yield 存活 → 也在帧里。
FrameDemo<int> frame_user(Trace param) {
    Trace local{"local"};  // 跨挂起 → 帧内
    co_yield 1;
    // 恢复后 param / local 仍有效(在帧里, 不在调用方栈)
    (void)param;
    (void)local;
    co_yield 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== coroutine_frame_layout ===\n";
    std::cout << "  creating coroutine (allocates frame on heap by default)...\n";

    {
        auto g = frame_user(Trace{"arg"});
        // 帧地址非空: handle 指向堆上协程帧
        assert(g.frame_address() != nullptr);
        std::cout << "  frame address = " << g.frame_address() << '\n';

        assert(g.next() && g.value() == 1);
        std::cout << "  after first yield: param+local still live in frame\n";
        assert(g.next() && g.value() == 2);
        assert(!g.next());
        std::cout << "  destroying FrameDemo → handle.destroy() cleans frame\n";
    }

    // 帧布局(概念图):
    //   ┌─────────────────────────────┐  heap
    //   │ promise (控制器)            │
    //   │ 参数副本 (by-value)         │  ← 引用参数只拷引用, 见 by_value topic
    //   │ 跨挂起局部变量              │
    //   │ 挂起点编号 / resume 跳转表  │
    //   └─────────────────────────────┘
    // HALO: 若编译器证明帧不逃逸, 可能把帧放调用者栈上(优化, 不保证)。

    std::cout << "coroutine_frame_layout: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section01/coroutine_frame_layout", run>;

}  // namespace
