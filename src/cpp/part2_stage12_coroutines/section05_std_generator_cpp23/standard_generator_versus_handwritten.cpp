// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : standard_generator_versus_handwritten
// Topic id : part2/stage12/section05/standard_generator_versus_handwritten
//
// Covers: std::generator vs handwritten Gen

#include "learn/topic_registry.hpp"

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

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto hand = []() -> Gen<int> { co_yield 1; }();
    LEARN_CHECK(hand.next() && hand.value() == 1);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    auto stdg = []() -> std::generator<int> {
        co_yield 1;
        co_yield 2;
    }();
    std::vector<int> a;
    for (int x : stdg) {
        a.push_back(x);
    }
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto hand = []() -> Gen<int> {
        co_yield 1;
        co_yield 2;
    }();
    std::vector<int> b;
    while (hand.next()) {
        b.push_back(hand.value());
    }
    LEARN_CHECK(a == b);
#else
    LEARN_CHECK((a == std::vector<int>{1, 2}));
#endif
#else
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto hand = []() -> Gen<int> {
        co_yield 1;
        co_yield 2;
    }();
    std::vector<int> b;
    while (hand.next()) {
        b.push_back(hand.value());
    }
    LEARN_CHECK((b == std::vector<int>{1, 2}));
#else
    LEARN_CHECK(true);
#endif
#endif
}

void demo_expert() {
    // Standard generator: ranges-friendly, recursive elements_of, allocator support.
    // Handwritten: minimal teaching model for promise/yield_value.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/standard_generator_versus_handwritten", run>;

}  // namespace
