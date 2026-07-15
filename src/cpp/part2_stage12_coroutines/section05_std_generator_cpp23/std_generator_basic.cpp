// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : std_generator_basic
// Topic id : part2/stage12/section05/std_generator_basic
//
// 要点: C++23 std::generator — 第一个标准协程类型;
//       co_yield 惰性产值, 自带迭代器, 直接 range-for。
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/generator
//   P2502R2  Feature-test: __cpp_lib_generator 202207L

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
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

std::generator<int> fibonacci() {
    int a = 0;
    int b = 1;
    for (;;) {
        co_yield a;
        const int next = a + b;
        a = b;
        b = next;
    }
}

std::generator<int> range(int lo, int hi) {
    for (int i = lo; i < hi; ++i) {
        co_yield i;
    }
}

#else

// 后备手写(标准库无 <generator> 时仍可教学运行)
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

FallbackGen<int> fibonacci() {
    int a = 0;
    int b = 1;
    for (;;) {
        co_yield a;
        const int next = a + b;
        a = b;
        b = next;
    }
}

FallbackGen<int> range(int lo, int hi) {
    for (int i = lo; i < hi; ++i) {
        co_yield i;
    }
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== std_generator_basic ===\n";
#if LEARN_HAS_STD_GENERATOR
    std::cout << "  using std::generator (__cpp_lib_generator=" << __cpp_lib_generator << ")\n";
#else
    std::cout << "  fallback MiniGen (no __cpp_lib_generator)\n";
#endif

    std::vector<int> fibs;
    int count = 0;
    for (int f : fibonacci()) {
        fibs.push_back(f);
        if (++count == 10) {
            break;  // 无限序列 + 惰性: 只取前 10
        }
    }
    assert((fibs == std::vector<int>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}));
    std::cout << "  fib first 10:";
    for (int x : fibs) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';

    std::vector<int> r;
    for (int x : range(3, 7)) {
        r.push_back(x);
    }
    assert((r == std::vector<int>{3, 4, 5, 6}));
    std::cout << "  range(3,7): 3 4 5 6\n";

    // cppreference: "synchronous coroutine generator for ranges"
    // —— 同步、惰性, 不是 async Task。

    std::cout << "std_generator_basic: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/std_generator_basic", run>;

}  // namespace
