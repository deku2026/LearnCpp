// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : elements_of_recursive_yield
// Topic id : part2/stage12/section05/elements_of_recursive_yield
//
// Covers: elements_of recursive yield (gated)

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

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
#include <ranges>

#if defined(__has_include)
#if __has_include(<generator>)
#include <generator>
#endif
#endif
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
    auto leaf = []() -> std::generator<int> {
        co_yield 1;
        co_yield 2;
    };
    auto tree = [&]() -> std::generator<int> {
        co_yield std::ranges::elements_of(leaf());
        co_yield 3;
    };
    std::vector<int> v;
    for (int x : tree()) {
        v.push_back(x);
    }
    LEARN_CHECK((v == std::vector<int>{1, 2, 3}));
#else
#if defined(__cpp_impl_coroutine) || defined(__cpp_coroutines)
    auto g = []() -> Gen<int> {
        co_yield 1;
        co_yield 2;
        co_yield 3;
    }();
    std::vector<int> v;
    while (g.next()) {
        v.push_back(g.value());
    }
    LEARN_CHECK((v == std::vector<int>{1, 2, 3}));
#else
    LEARN_CHECK(true);
#endif
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
    auto nested = [](auto& self, int n) -> std::generator<int> {
        if (n <= 0) {
            co_return;
        }
        co_yield n;
        co_yield std::ranges::elements_of(self(self, n - 1));
    };
    std::vector<int> v;
    for (int x : nested(nested, 3)) {
        v.push_back(x);
    }
    LEARN_CHECK((v == std::vector<int>{3, 2, 1}));
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
    // elements_of flattens a nested generator without manual loops.
    auto a = []() -> std::generator<int> { co_yield 7; };
    auto b = [&]() -> std::generator<int> { co_yield std::ranges::elements_of(a()); };
    int x = 0;
    for (int v : b()) {
        x = v;
    }
    LEARN_CHECK(x == 7);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/elements_of_recursive_yield", run>;

}  // namespace
