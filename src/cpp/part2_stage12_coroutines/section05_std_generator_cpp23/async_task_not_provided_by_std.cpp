// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : async_task_not_provided_by_std
// Topic id : part2/stage12/section05/async_task_not_provided_by_std
//
// 要点: std::generator 是【同步】生成器 — 不提供调度器/异步 IO/取消/co_await Task。
//       异步任务须自造(见 section06)或第三方; 标准异步模型指向 C++26 std::execution。
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/generator
//   P2300 std::execution (future)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#define LEARN_HAS_STD_GENERATOR 1
#endif

namespace {

// generator 能做的: co_yield 序列
#if defined(LEARN_HAS_STD_GENERATOR)
std::generator<int> only_sync_yield() {
    co_yield 1;
    co_yield 2;
}
#endif

// 标准【没有】的 Task — 自造最小版证明“机制够用, 库类型缺失”
template <typename T>
struct DiyTask {
    struct promise_type {
        std::optional<T> result;
        std::coroutine_handle<> cont{};
        DiyTask get_return_object() { return DiyTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
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
    explicit DiyTask(Handle h) noexcept : h_(h) {}
    ~DiyTask() {
        if (h_) {
            h_.destroy();
        }
    }
    DiyTask(DiyTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    DiyTask(const DiyTask&) = delete;

    T sync() {
        h_.resume();
        return std::move(*h_.promise().result);
    }
    auto operator co_await() {
        struct Aw {
            Handle c;
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
                c.promise().cont = caller;
                return c;
            }
            T await_resume() { return std::move(*c.promise().result); }
        };
        return Aw{h_};
    }

private:
    Handle h_{};
};

DiyTask<int> async_style_add(int x) {
    // 这里的 “async” 只是 co_await 链, 无线程/IO —— 但语法上是 Task 模型
    co_return x + 1;
}

DiyTask<int> pipeline() {
    int y = co_await async_style_add(41);
    co_return y;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== async_task_not_provided_by_std ===\n";

#if defined(LEARN_HAS_STD_GENERATOR)
    std::vector<int> g;
    for (int x : only_sync_yield()) {
        g.push_back(x);
    }
    assert((g == std::vector<int>{1, 2}));
    std::cout << "  std::generator: sync yield OK\n";
#else
    std::cout << "  (std::generator unavailable on this STL)\n";
#endif

    auto t = pipeline();
    assert(t.sync() == 42);
    std::cout << "  DIY Task co_await chain: 42 (not in std C++23)\n";

    std::cout << "  std::generator does NOT provide:\n";
    std::cout << "    - thread pool / scheduler\n";
    std::cout << "    - async IO awaitables\n";
    std::cout << "    - cancellation / timeout / executor\n";
    std::cout << "    - general co_await Task type\n";
    std::cout << "  C++23 = generator standardized; async Task still library/C++26.\n";

    // 🔶 Boost.Asio: asio::awaitable<T> + co_spawn 填补“异步 Task+executor”空缺
    // 🔶 Boost.Coroutine2: stackful 纤程模型, 与 C++20 stackless 协程不同代际

    std::cout << "async_task_not_provided_by_std: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/async_task_not_provided_by_std", run>;

}  // namespace
