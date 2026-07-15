// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : what_is_a_coroutine
// Topic id : part2/stage12/section01/what_is_a_coroutine
//
// Covers: coroutine = function with co_await/co_yield/co_return

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>
#include <vector>

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
Gen<int> count_to(int n) {
    for (int i = 0; i < n; ++i) {
        co_yield i;
    }
}
#endif

void demo_basics() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto g = count_to(3);
    assert(g.next());
    assert(g.value() == 0);
    assert(g.next());
    assert(g.value() == 1);
    assert(g.next());
    assert(g.value() == 2);
    assert(!g.next());
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    std::vector<int> v;
    auto g = count_to(4);
    while (g.next()) {
        v.push_back(g.value());
    }
    assert((v == std::vector<int>{0, 1, 2, 3}));
#else
    assert(true);
#endif
}

void demo_expert() {
    // Not a coroutine: ordinary function returns all at once.
    auto plain = [] { return std::vector<int>{0, 1, 2}; };
    assert(plain().size() == 3);
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto g = count_to(3);
    int n = 0;
    while (g.next()) {
        ++n;
    }
    assert(n == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section01/what_is_a_coroutine", run>;

}  // namespace
