// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : return_value_or_return_void
// Topic id : part2/stage12/section02/return_value_or_return_void
//
// Covers: return_void vs return_value

#include "learn/topic_registry.hpp"

#include <optional>
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
struct IntTask {
    struct promise_type {
        std::optional<int> result;
        IntTask get_return_object() { return IntTask{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { result = v; }
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> h{};
    explicit IntTask(std::coroutine_handle<promise_type> handle) : h(handle) {}
    IntTask(IntTask&& o) noexcept : h(std::exchange(o.h, {})) {}
    ~IntTask() {
        if (h) {
            h.destroy();
        }
    }
    IntTask(const IntTask&) = delete;
    int get() const { return *h.promise().result; }
};

IntTask answer() {
    co_return 42;
}

Resumable voidy(int* p) {
    *p = 1;
    co_return;
}
#endif

void demo_basics() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto t = answer();
    LEARN_CHECK(t.get() == 42);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    int n = 0;
    auto r = voidy(&n);
    r.resume();
    LEARN_CHECK(n == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    // co_return x => return_value; bare co_return => return_void.
    auto t = answer();
    LEARN_CHECK(t.h.done());
    LEARN_CHECK(t.get() == 42);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/return_value_or_return_void", run>;

}  // namespace
