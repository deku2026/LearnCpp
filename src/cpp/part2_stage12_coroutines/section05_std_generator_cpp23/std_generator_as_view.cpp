// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : std_generator_as_view
// Topic id : part2/stage12/section05/std_generator_as_view
//
// 要点: std::generator 满足 view + input_range, 可直接接 ranges 管道。
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/generator
//   https://en.cppreference.com/w/cpp/ranges

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#define LEARN_HAS_STD_GENERATOR 1
#else
#define LEARN_HAS_STD_GENERATOR 0
#include <coroutine>
#endif

namespace {

#if LEARN_HAS_STD_GENERATOR

std::generator<int> naturals() {
    for (int i = 1;; ++i) {
        co_yield i;
    }
}

#else

template <typename T>
struct FallbackGen {
    struct promise_type {
        T current{};
        FallbackGen get_return_object() {
            return FallbackGen{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit FallbackGen(Handle h) noexcept : h_(h) {}
    ~FallbackGen() {
        if (h_) {
            h_.destroy();
        }
    }
    FallbackGen(FallbackGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    FallbackGen(const FallbackGen&) = delete;

    struct iterator {
        Handle h{};
        void operator++() { h.resume(); }
        const T& operator*() const { return h.promise().current; }
        bool operator==(std::default_sentinel_t) const { return !h || h.done(); }
    };
    iterator begin() {
        if (h_) {
            h_.resume();
        }
        return iterator{h_};
    }
    std::default_sentinel_t end() const noexcept { return {}; }

private:
    Handle h_{};
};

FallbackGen<int> naturals() {
    for (int i = 1;; ++i) {
        co_yield i;
    }
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== std_generator_as_view ===\n";

    // 无限自然数 | 滤偶数 | 平方 | 取 5
    auto pipe = naturals() | std::views::filter([](int x) { return x % 2 == 0; }) |
                std::views::transform([](int x) { return x * x; }) | std::views::take(5);

    std::vector<int> out;
    for (int x : pipe) {
        out.push_back(x);
    }
    assert((out == std::vector<int>{4, 16, 36, 64, 100}));

    std::cout << "  naturals|filter even|square|take5 →";
    for (int x : out) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';

#if LEARN_HAS_STD_GENERATOR
    static_assert(std::ranges::view<std::generator<int>>);
    static_assert(std::ranges::input_range<std::generator<int>>);
    std::cout << "  std::generator is view + input_range\n";
#endif

    // 意义: 协程顺序写法(co_yield) + Ranges 声明式管道 = 惰性组合。

    std::cout << "std_generator_as_view: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/std_generator_as_view", run>;

}  // namespace
