// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : stackless_state_machine
// Topic id : part2/stage12/section01/stackless_state_machine
//
// 要点: 编译器把协程改写成状态机——以挂起点切段, 状态编号记断点;
//       stackless = 无独立调用栈, 跨挂起状态全在堆上的协程帧。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   Lewis Baker: Understanding the Compiler Transform

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// ① 手写“状态机”伪实现: 模拟编译器改写后的控制流(教学用, 非真帧布局)。
// ---------------------------------------------------------------------------
struct HandWrittenSm {
    enum class State { Start, AfterYieldA, AfterYieldB, Done };
    State st = State::Start;
    int a = 0;
    int b = 0;
    int out = 0;

    // 每次调用 = 一次 resume: 按状态跳到对应“段”
    bool resume() {
        switch (st) {
            case State::Start:
                a = 10;
                out = a;
                st = State::AfterYieldA;
                return true;  // 模拟 co_yield a
            case State::AfterYieldA:
                b = a + 1;
                out = b;
                st = State::AfterYieldB;
                return true;  // 模拟 co_yield b
            case State::AfterYieldB:
                st = State::Done;
                return false;
            case State::Done:
                return false;
        }
        return false;
    }
};

// ---------------------------------------------------------------------------
// ② 真协程: 编译器自动生成等价状态机 + 协程帧。
// ---------------------------------------------------------------------------
template <typename T>
struct SmGen {
    struct promise_type {
        T current{};
        SmGen get_return_object() { return SmGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
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
    explicit SmGen(Handle h) noexcept : h_(h) {}
    ~SmGen() {
        if (h_) {
            h_.destroy();
        }
    }
    SmGen(SmGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    SmGen(const SmGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().current; }

private:
    Handle h_{};
};

// 源码像顺序代码; 编译器切成: Start → yield a → yield b → Done
SmGen<int> two_yields() {
    int a = 10;     // 跨挂起点 → 进协程帧
    co_yield a;     // 状态点 1
    int b = a + 1;  // 恢复后 a 仍在帧里
    co_yield b;     // 状态点 2
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== stackless_state_machine ===\n";

    // --- 手写状态机路径 ---
    [[maybe_unused]] HandWrittenSm sm;
    assert(sm.resume() && sm.out == 10);
    assert(sm.resume() && sm.out == 11);
    assert(!sm.resume());
    std::cout << "  handwritten SM: 10, 11, done\n";

    // --- 真协程路径(输出应一致) ---
    auto g = two_yields();
    assert(g.next() && g.value() == 10);
    assert(g.next() && g.value() == 11);
    assert(!g.next());
    std::cout << "  coroutine:      10, 11, done\n";

    // stackless vs stackful (Boost.Coroutine2 / fiber 教学对比, 不链接库):
    //   stackful: 每协程一整块独立栈, 可在任意嵌套普通函数里挂起, 内存大。
    //   stackless(C++20): 只在本协程体挂起, 帧里只存必要状态, 切换快、省内存。
    //
    // 改写伪代码骨架(见文档步骤 2.3):
    //   new frame → construct promise → get_return_object
    //   → co_await initial_suspend → body → final_suspend

    std::cout << "stackless_state_machine: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section01/stackless_state_machine", run>;

}  // namespace
