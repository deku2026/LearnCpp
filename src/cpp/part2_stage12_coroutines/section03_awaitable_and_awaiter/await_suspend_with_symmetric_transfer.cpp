// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : await_suspend_with_symmetric_transfer
// Topic id : part2/stage12/section03/await_suspend_with_symmetric_transfer
//
// 要点: await_suspend 三种返回:
//   void     → 保持挂起, 回到恢复者
//   bool     → true 挂起 / false 立即恢复当前
//   handle   → ⭐ 对称转移: 尾调用恢复返回的协程, 栈深度恒定
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   Lewis Baker: Understanding Symmetric Transfer
//   https://en.cppreference.com/w/cpp/coroutine/noop_coroutine

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace {

std::vector<std::string> g_log;

// ---------------------------------------------------------------------------
// 完整最小 Task: await_suspend 返回 handle = 对称转移
// ---------------------------------------------------------------------------
template <typename T>
class Task {
public:
    struct promise_type {
        std::optional<T> result;
        std::exception_ptr ep;
        std::coroutine_handle<> continuation{};

        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                g_log.push_back("final_suspend→transfer");
                auto c = h.promise().continuation;
                return c ? c : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }

        void return_value(T v) { result = std::move(v); }
        void unhandled_exception() { ep = std::current_exception(); }
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

    T sync() {
        h_.resume();
        assert(h_.done());
        if (h_.promise().ep) {
            std::rethrow_exception(h_.promise().ep);
        }
        return std::move(*h_.promise().result);
    }

    auto operator co_await() && {
        struct Awaiter {
            Handle callee;
            bool await_ready() const noexcept { return false; }

            // ⭐ 返回 callee → 编译器尾调用 resume(callee), 不嵌套压栈
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
                g_log.push_back("await_suspend→transfer to callee");
                callee.promise().continuation = caller;
                return callee;
            }

            // 对比 naive(会栈累积, 切勿在深链用):
            //   void await_suspend(coroutine_handle<> caller) {
            //       callee.promise().continuation = caller;
            //       callee.resume();  // 普通调用, 栈 +1
            //   }

            T await_resume() {
                if (callee.promise().ep) {
                    std::rethrow_exception(callee.promise().ep);
                }
                return std::move(*callee.promise().result);
            }
        };
        return Awaiter{h_};
    }

private:
    Handle h_{};
};

// bool 返回: 演示 “其实已就绪”
struct MaybeReady {
    bool already;
    int v;
    bool await_ready() const noexcept { return false; }  // 先假装要挂
    bool await_suspend(std::coroutine_handle<>) const noexcept {
        g_log.push_back(already ? "bool_suspend=false(cancel)" : "bool_suspend=true");
        return !already;  // false → 立即恢复当前协程
    }
    int await_resume() const noexcept { return v; }
};

Task<int> leaf() {
    co_return 3;
}

Task<int> mid() {
    int x = co_await std::move(leaf());
    co_return x + 4;
}

Task<int> top() {
    int y = co_await std::move(mid());
    co_return y * 2;
}

struct BoolTask {
    struct promise_type {
        int r = 0;
        BoolTask get_return_object() { return BoolTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { r = v; }
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit BoolTask(Handle h) noexcept : h_(h) {}
    ~BoolTask() {
        if (h_) {
            h_.destroy();
        }
    }
    BoolTask(BoolTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    BoolTask(const BoolTask&) = delete;
    int run() {
        h_.resume();
        return h_.promise().r;
    }

private:
    Handle h_{};
};

BoolTask bool_path_safe() {
    // already=true → await_suspend 返回 false → 取消挂起, 立刻 await_resume
    int a = co_await MaybeReady{true, 7};
    int b = co_await MaybeReady{true, 1};
    co_return a + b;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== await_suspend_with_symmetric_transfer ===\n";
    g_log.clear();

    {
        auto t = top();
        assert(t.sync() == 14);  // (3+4)*2
        // 链: top→mid→leaf, final 再一路 transfer 回来
        int transfers = 0;
        for (const auto& s : g_log) {
            if (s.find("transfer") != std::string::npos) {
                ++transfers;
            }
            std::cout << "    " << s << '\n';
        }
        assert(transfers >= 4);  // 2 次 co_await 启动 + 2 次 final(至少)
        std::cout << "  symmetric transfer chain result 14\n";
    }

    g_log.clear();
    {
        auto t = bool_path_safe();
        assert(t.run() == 8);
        std::cout << "  await_suspend bool=false cancels suspend\n";
    }

    // Boost.Asio awaitable 教学对比(不链接):
    //   asio::awaitable<T> 的 co_await 同样靠 await_suspend 把续体挂到 executor;
    //   完成时 post 恢复 —— 与本文件 “continuation + transfer” 同构, 只是调度器换成 io_context。

    std::cout << "await_suspend_with_symmetric_transfer: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/await_suspend_with_symmetric_transfer", run>;

}  // namespace
