// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : async_task_not_provided_by_std
// Topic id : part2/stage12/section05/async_task_not_provided_by_std
//
// Covers: async Task not in std until execution (hand-rolled)

#include "learn/topic_registry.hpp"

#include <utility>

#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
#include <coroutine>
#include <exception>
#include <optional>
#include <utility>
#include <vector>

struct VoidTask {
    struct promise_type {
        VoidTask get_return_object() { return VoidTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> h{};
    explicit VoidTask(std::coroutine_handle<promise_type> handle) : h(handle) {}
    VoidTask(VoidTask&& o) noexcept : h(std::exchange(o.h, {})) {}
    ~VoidTask() {
        if (h) {
            h.destroy();
        }
    }
    VoidTask(const VoidTask&) = delete;
    VoidTask& operator=(const VoidTask&) = delete;
    bool done() const { return !h || h.done(); }
    void resume() {
        if (h && !h.done()) {
            h.resume();
        }
    }
};

template <class T>
struct Gen {
    struct promise_type {
        std::optional<T> current;
        Gen get_return_object() { return Gen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> h{};
    explicit Gen(std::coroutine_handle<promise_type> handle) : h(handle) {}
    Gen(Gen&& o) noexcept : h(std::exchange(o.h, {})) {}
    ~Gen() {
        if (h) {
            h.destroy();
        }
    }
    Gen(const Gen&) = delete;
    Gen& operator=(const Gen&) = delete;
    bool next() {
        if (!h || h.done()) {
            return false;
        }
        h.resume();
        return !h.done();
    }
    T value() const { return *h.promise().current; }
};

struct Resumable {
    struct promise_type {
        Resumable get_return_object() { return Resumable{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> h{};
    explicit Resumable(std::coroutine_handle<promise_type> handle) : h(handle) {}
    Resumable(Resumable&& o) noexcept : h(std::exchange(o.h, {})) {}
    ~Resumable() {
        if (h) {
            h.destroy();
        }
    }
    Resumable(const Resumable&) = delete;
    Resumable& operator=(const Resumable&) = delete;
    void resume() {
        if (h && !h.done()) {
            h.resume();
        }
    }
    bool done() const { return !h || h.done(); }
};
#endif

namespace {

#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
struct MiniTask {
    struct promise_type {
        MiniTask get_return_object() { return MiniTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> h{};
    explicit MiniTask(std::coroutine_handle<promise_type> handle) : h(handle) {}
    MiniTask(MiniTask&& o) noexcept : h(std::exchange(o.h, {})) {}
    ~MiniTask() {
        if (h) {
            h.destroy();
        }
    }
    MiniTask(const MiniTask&) = delete;
    void start() {
        if (h) {
            h.resume();
        }
    }
    bool done() const { return !h || h.done(); }
};

MiniTask delay_set(int* p) {
    co_await std::suspend_always{};
    *p = 1;
}
#endif

void demo_basics() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    int n = 0;
    auto t = delay_set(&n);
    LEARN_CHECK(n == 0);
    t.start();
    LEARN_CHECK(n == 0);
    t.start();
    LEARN_CHECK(n == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
    // C++23 provides std::generator, not a general async Task.
    // Async execution model trends toward C++26 std::execution.
    LEARN_CHECK(true);
}

void demo_expert() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    int n = 0;
    auto t = delay_set(&n);
    while (!t.done()) {
        t.start();
    }
    LEARN_CHECK(n == 1);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/async_task_not_provided_by_std", run>;

}  // namespace
