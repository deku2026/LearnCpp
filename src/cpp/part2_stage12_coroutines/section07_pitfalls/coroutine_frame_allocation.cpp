// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : coroutine_frame_allocation
// Topic id : part2/stage12/section07/coroutine_frame_allocation
//
// Covers: frame allocation (new) can be customized

#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <new>
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
struct Tracked {
    struct promise_type {
        static int allocs;
        static void* operator new(std::size_t n) {
            ++allocs;
            return ::operator new(n);
        }
        static void operator delete(void* p, std::size_t) noexcept {
            --allocs;
            ::operator delete(p);
        }
        Tracked get_return_object() { return Tracked{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> h{};
    explicit Tracked(std::coroutine_handle<promise_type> handle) : h(handle) {}
    Tracked(Tracked&& o) noexcept : h(std::exchange(o.h, {})) {}
    ~Tracked() {
        if (h) {
            h.destroy();
        }
    }
    Tracked(const Tracked&) = delete;
};
int Tracked::promise_type::allocs = 0;

Tracked make_tracked() {
    co_return;
}
#endif

void demo_basics() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    Tracked::promise_type::allocs = 0;
    {
        auto t = make_tracked();
        LEARN_CHECK(Tracked::promise_type::allocs == 1);
    }
    LEARN_CHECK(Tracked::promise_type::allocs == 0);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    Tracked::promise_type::allocs = 0;
    auto t = make_tracked();
    LEARN_CHECK(Tracked::promise_type::allocs == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    // promise_type operator new/delete customizes frame allocation.
    Tracked::promise_type::allocs = 0;
    auto t1 = make_tracked();
    auto t2 = make_tracked();
    LEARN_CHECK(Tracked::promise_type::allocs == 2);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/coroutine_frame_allocation", run>;

}  // namespace
