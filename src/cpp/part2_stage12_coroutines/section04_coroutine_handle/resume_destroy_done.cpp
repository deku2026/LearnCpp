// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section04_coroutine_handle
// Item     : resume_destroy_done
// Topic id : part2/stage12/section04/resume_destroy_done
//
// 要点:
//   resume()/operator()  — 从挂起点继续; done() 后 resume → UB
//   done()               — 是否已到 final_suspend
//   destroy()            — 析构帧; 只能销毁【挂起】的协程
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/coroutine_handle

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

namespace {

struct Tracer {
    const char* tag;
    explicit Tracer(const char* t) : tag(t) { std::cout << "    +" << tag << '\n'; }
    ~Tracer() { std::cout << "    -" << tag << '\n'; }
    Tracer(const Tracer&) = delete;
};

struct Ctrl {
    struct promise_type {
        int n = 0;
        Ctrl get_return_object() { return Ctrl{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int v) {
            n = v;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Ctrl(Handle h) noexcept : h_(h) {}
    // 本文件手动 destroy 演示: 默认不 auto-destroy, 由 run 控制
    ~Ctrl() {
        if (h_) {
            h_.destroy();
        }
    }
    Ctrl(Ctrl&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Ctrl(const Ctrl&) = delete;

    Handle release() noexcept { return std::exchange(h_, {}); }
    Handle get() const noexcept { return h_; }

private:
    Handle h_{};
};

Ctrl three_steps() {
    Tracer live{"frame-local"};
    co_yield 1;
    co_yield 2;
    co_yield 3;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== resume_destroy_done ===\n";

    // --- 正常 resume 直到 done ---
    {
        auto c = three_steps();
        auto h = c.get();
        assert(!h.done());

        h.resume();  // → yield 1
        assert(!h.done() && h.promise().n == 1);
        h();  // operator() 等价 resume → yield 2
        assert(!h.done() && h.promise().n == 2);
        h.resume();  // → yield 3
        assert(!h.done() && h.promise().n == 3);
        h.resume();  // → final
        assert(h.done());
        std::cout << "  resume sequence → done() true at final_suspend\n";
        // ⚠️ 此时再 h.resume() 是 UB —— 不演示
    }

    // --- 中途 destroy: 析构仍存活的帧内局部 ---
    {
        std::cout << "  destroy mid-way (local dtor should run):\n";
        auto c = three_steps();
        auto h = c.release();  // 接管所有权
        h.resume();            // 创建 Tracer, 停在 yield 1
        assert(!h.done());
        h.destroy();  // 销毁挂起的协程 → Tracer 析构
        // h 此后悬垂, 不得再用
        std::cout << "  mid-way destroy OK\n";
    }

    // 规则备忘:
    //  - resume 正在运行的协程 → UB
    //  - destroy 正在运行的协程 → UB
    //  - destroy 后使用 handle → UB

    std::cout << "resume_destroy_done: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section04/resume_destroy_done", run>;

}  // namespace
