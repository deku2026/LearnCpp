// Topic    : C++23 std::generator 在 STL/Ranges 语境下的回顾
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.3
// Stage    : part2_stage10_stl_deep_dive
// Section  : section12_coroutine_library
// Item     : std_generator_cpp23_recap
// Topic id : part2/stage10/section12/std_generator_cpp23_recap
// Refs     : https://en.cppreference.com/w/cpp/coroutine/generator
//            https://en.cppreference.com/w/cpp/ranges
//            P2502R2  Feature-test: __cpp_lib_generator

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

std::generator<int> count_from(int start) {
    for (int i = start;; ++i) {
        co_yield i;
    }
}

std::generator<int> countdown(int n) {
    while (n > 0) {
        co_yield n;
        --n;
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

FallbackGen<int> count_from(int start) {
    for (int i = start;; ++i) {
        co_yield i;
    }
}

FallbackGen<int> countdown(int n) {
    while (n > 0) {
        co_yield n;
        --n;
    }
}

#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_generator_cpp23_recap] generator as view ===\n";
#if LEARN_HAS_STD_GENERATOR
    std::cout << "  using std::generator (__cpp_lib_generator=" << __cpp_lib_generator << ")\n";
#else
    std::cout << "  fallback MiniGen (no __cpp_lib_generator)\n";
#endif

    // ① 有限序列 + range-for
    {
        std::vector<int> out;
        for (int x : countdown(4)) {
            out.push_back(x);
        }
        assert((out == std::vector<int>{4, 3, 2, 1}));
        std::cout << "  countdown(4) → 4 3 2 1\n";
    }

    // ② STL 角度: generator 是 input_range / view, 直接进 Ranges 管道
    //    文档示例: makeGenerator() | filter | take
    {
        auto pipe = count_from(1) | std::views::filter([](int x) { return x % 2 == 0; }) |
                    std::views::transform([](int x) { return x * x; }) | std::views::take(5);

        std::vector<int> out;
        for (int x : pipe) {
            out.push_back(x);
        }
        assert((out == std::vector<int>{4, 16, 36, 64, 100}));
        std::cout << "  count_from|even|square|take5 →";
        for (int x : out) {
            std::cout << ' ' << x;
        }
        std::cout << '\n';
    }

#if LEARN_HAS_STD_GENERATOR
    static_assert(std::ranges::view<std::generator<int>>);
    static_assert(std::ranges::input_range<std::generator<int>>);
    // 单遍 input: 不要假设可反复遍历同一 generator 实例
    std::cout << "  static_assert: generator is view + input_range\n";
#endif

    // ③ 与 ranges::to 物化(若可用)
#if LEARN_HAS_STD_GENERATOR && defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
    {
        auto v = countdown(3) | std::ranges::to<std::vector>();
        assert((v == std::vector<int>{3, 2, 1}));
        std::cout << "  ranges::to materialize countdown\n";
    }
#endif

    // 阶段 12 深讲协程机制; 本阶段只需记住:
    // generator = 标准库第一个协程类型 = 可接 Ranges 的惰性 input view
    std::cout << "[std_generator_cpp23_recap] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section12/std_generator_cpp23_recap", run>;

}  // namespace
