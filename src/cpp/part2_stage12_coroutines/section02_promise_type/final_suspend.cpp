// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : final_suspend
// Topic id : part2/stage12/section02/final_suspend
//
// 要点: final_suspend 在 body/异常处理后 co_await, 必须 noexcept;
//   suspend_always → 停在最终挂起点, 外部 done()==true, 再手动 destroy;
//   返回对称转移 awaiter → 恢复 continuation(Task 模式)。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   https://en.cppreference.com/w/cpp/coroutine/noop_coroutine

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

namespace {

// ---------------------------------------------------------------------------
// A) Generator 风格: final_suspend = suspend_always
// ---------------------------------------------------------------------------
struct AlwaysFinal {
    struct promise_type {
        int v = 0;
        AlwaysFinal get_return_object() {
            return AlwaysFinal{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        // ⚠️ 必须 noexcept
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int x) {
            v = x;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit AlwaysFinal(Handle h) noexcept : h_(h) {}
    ~AlwaysFinal() {
        if (h_) {
            h_.destroy();
        }
    }
    AlwaysFinal(AlwaysFinal&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    AlwaysFinal(const AlwaysFinal&) = delete;

    void step() { h_.resume(); }
    bool done() const noexcept { return h_.done(); }
    int value() const { return h_.promise().v; }

private:
    Handle h_{};
};

AlwaysFinal gen_one() {
    co_yield 5;
}

// ---------------------------------------------------------------------------
// B) Task 风格: final_suspend 对称转移恢复续体
// ---------------------------------------------------------------------------
struct ContTask {
    struct promise_type {
        int result = 0;
        std::coroutine_handle<> cont{};

        ContTask get_return_object() { return ContTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                auto c = h.promise().cont;
                // 无续体 → noop, 控制权回到顶层 resume 的调用者
                return c ? c : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(int x) { result = x; }
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit ContTask(Handle h) noexcept : h_(h) {}
    ~ContTask() {
        if (h_) {
            h_.destroy();
        }
    }
    ContTask(ContTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ContTask(const ContTask&) = delete;

    int sync() {
        h_.resume();
        assert(h_.done());
        return h_.promise().result;
    }

    auto operator co_await() {
        struct Awaiter {
            Handle cal;
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
                cal.promise().cont = caller;
                return cal;
            }
            int await_resume() const { return cal.promise().result; }
        };
        return Awaiter{h_};
    }

private:
    Handle h_{};
};

ContTask produce() {
    co_return 11;
}

ContTask consume() {
    int x = co_await produce();
    co_return x + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== final_suspend ===\n";

    {
        auto g = gen_one();
        g.step();  // → yield
        assert(!g.done());
        g.step();  // → final_suspend(always)
        assert(g.done());
        assert(g.value() == 5);
        // 帧仍在, 析构时 destroy —— 这就是 suspend_always final 的价值
        std::cout << "  suspend_always final: done() true, frame kept for RAII destroy\n";
    }

    {
        auto t = consume();
        assert(t.sync() == 12);
        std::cout << "  FinalAwaiter symmetric transfer: chain result 12\n";
    }

    std::cout << "final_suspend: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/final_suspend", run>;

}  // namespace
