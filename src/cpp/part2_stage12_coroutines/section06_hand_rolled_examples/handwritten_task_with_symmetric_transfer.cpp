// LearnCpp topic
// Doc      : 第2部分-阶段12-协程.md · 步骤 9 / 10（手写 Task + 对称转移）
// Stage    : part2_stage12_coroutines
// Section  : section06_hand_rolled_examples
// Item     : handwritten_task_with_symmetric_transfer
// Topic id : part2/stage12/section06/handwritten_task_with_symmetric_transfer
//
// 对照（教学注释，不强制链接）:
//   Boost.Asio asio::awaitable + symmetric transfer 思路同类;
//   真正异步还需 executor（io_context），本文件只演示语言层 Task 骨架。
//   Boost.Coroutine2 是 stackful，与这里 stackless promise 模型不同。
//
// 要点: 完整最小 Task<T> — continuation + final_suspend/operator co_await
//       均用 await_suspend 返回 handle 做对称转移。
// Refs:
//   Lewis Baker: Understanding Symmetric Transfer
//   https://en.cppreference.com/w/cpp/coroutine/noop_coroutine
//   文档步骤 9–10

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

template <typename T>
class Task {
public:
    struct promise_type {
        std::optional<T> result_;
        std::exception_ptr exception_;
        std::coroutine_handle<> continuation_{};

        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }  // 懒: 先挂 cont

        struct FinalAwaiter {
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                auto cont = h.promise().continuation_;
                return cont ? cont : std::noop_coroutine();
            }
            void await_resume() noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(T v) { result_ = std::move(v); }
        void unhandled_exception() { exception_ = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Task(Handle h) : handle_(h) {}
    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }
    Task(const Task&) = delete;
    Task(Task&& o) noexcept : handle_(std::exchange(o.handle_, {})) {}

    // 顶层同步驱动
    T get() {
        handle_.resume();
        assert(handle_.done());
        if (handle_.promise().exception_) {
            std::rethrow_exception(handle_.promise().exception_);
        }
        return std::move(*handle_.promise().result_);
    }

    auto operator co_await() {
        struct Awaiter {
            Handle callee_;
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
                callee_.promise().continuation_ = caller;
                return callee_;  // ⭐ 对称转移去执行被等 Task
            }
            T await_resume() {
                if (callee_.promise().exception_) {
                    std::rethrow_exception(callee_.promise().exception_);
                }
                return std::move(*callee_.promise().result_);
            }
        };
        return Awaiter{handle_};
    }

private:
    Handle handle_{};
};

Task<int> leaf(int x) {
    co_return x;
}

Task<int> mid(int x) {
    int a = co_await leaf(x);
    int b = co_await leaf(1);
    co_return a + b;
}

Task<std::string> label(int n) {
    int v = co_await mid(n);
    co_return "v=" + std::to_string(v);
}

Task<int> boom() {
    throw std::runtime_error("task-boom");
    co_return 0;
}

Task<int> catcher() {
    try {
        co_return co_await boom();
    } catch (const std::runtime_error&) {
        co_return -1;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== handwritten_task_with_symmetric_transfer ===\n";

    {
        auto t = mid(10);
        assert(t.get() == 11);
        std::cout << "  mid(10) = 11\n";
    }
    {
        auto t = label(5);
        assert(t.get() == "v=6");
        std::cout << "  label(5) = v=6\n";
    }
    {
        auto t = catcher();
        assert(t.get() == -1);
        std::cout << "  exception through co_await → caught in caller task\n";
    }

    // 对称转移为何必要(文档步骤 10):
    //   naive callee.resume() 在 await_suspend 里嵌套调用 → 深链栈溢出
    //   return callee handle → 编译器尾调用, 栈深度 O(1)

    std::cout << "handwritten_task_with_symmetric_transfer: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage12/section06/handwritten_task_with_symmetric_transfer", run>;

}  // namespace
