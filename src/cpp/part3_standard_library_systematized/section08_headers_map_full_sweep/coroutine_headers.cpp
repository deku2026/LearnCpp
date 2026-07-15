// Topic    : headers map · 协程 coroutine
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : coroutine_headers
// Topic id : part3/section08/coroutine_headers
// Headers  : <coroutine> <generator>(C++23)

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <exception>
#include <iostream>
#include <optional>
#include <utility>
#include <version>

#if defined(__has_include)
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR 1
#endif
#endif
#ifndef LEARN_HAS_GENERATOR
#define LEARN_HAS_GENERATOR 0
#endif

#if !defined(__cpp_lib_generator) || !(__cpp_lib_generator)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::generator not available (__cpp_lib_generator)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/coroutine_headers", run>;
}  // namespace
#else
namespace {

// 最小可恢复任务：探测 <coroutine> 设施
struct Task {
    struct promise_type {
        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> h{};
    explicit Task(std::coroutine_handle<promise_type> handle) : h{handle} {}
    Task(Task&& other) noexcept : h{std::exchange(other.h, {})} {}
    ~Task() {
        if (h) {
            h.destroy();
        }
    }
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
};

Task minimal_coro(int& out) {
    out = 42;
    co_return;
}

#if LEARN_HAS_GENERATOR
std::generator<int> gen_two() {
    co_yield 1;
    co_yield 2;
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [coroutine_headers] map probe ===\n";
    std::cout << "<coroutine>  coroutine_handle / suspend_always / suspend_never\n";
    std::cout << "<generator>  C++23 std::generator (view + coroutine)\n";

    int value = 0;
    {
        Task t = minimal_coro(value);
        // initial_suspend never → 已跑到 final_suspend
        assert(value == 42);
        assert(t.h.done());
        std::cout << "minimal coroutine set value=" << value << '\n';
    }

#if LEARN_HAS_GENERATOR
    int sum = 0;
    for (int x : gen_two()) {
        sum += x;
    }
    assert(sum == 3);
    std::cout << "std::generator sum=" << sum << '\n';
#else
    std::cout << "<generator> not available; hand-rolled Task used above\n";
#endif

    std::cout << "[coroutine_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/coroutine_headers", run>;

}  // namespace
#endif  // __cpp_lib_generator
