// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : destructor_cannot_be_a_coroutine
// Topic id : part2/stage12/section07/destructor_cannot_be_a_coroutine
//
// Covers: ctors/dtors/main/constexpr cannot be coroutines

#include "learn/topic_registry.hpp"

#include <type_traits>
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

void demo_basics() {
    // Language rule: constructors, destructors, main, constexpr functions
    // cannot be coroutines. Demonstrate ordinary destructor instead.
    struct S {
        int* p;
        explicit S(int* q) : p(q) { *p = 1; }
        ~S() { *p = 2; }
    };
    int n = 0;
    {
        S s(&n);
        LEARN_CHECK(n == 1);
    }
    LEARN_CHECK(n == 2);
}

void demo_intermediate() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    // Regular member functions may be coroutines; special members may not.
    struct Holder {
        Resumable go(int* p) {
            *p = 3;
            co_return;
        }
    };
    int n = 0;
    Holder h;
    auto r = h.go(&n);
    r.resume();
    LEARN_CHECK(n == 3);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
    static_assert(std::is_trivially_destructible_v<int>);
    // Keep cleanup in normal destructors; start coroutines from ordinary functions.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/destructor_cannot_be_a_coroutine", run>;

}  // namespace
