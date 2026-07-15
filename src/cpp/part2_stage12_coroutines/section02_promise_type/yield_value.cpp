// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : yield_value
// Topic id : part2/stage12/section02/yield_value
//
// 要点: co_yield expr → co_await promise.yield_value(expr);
//       通常缓存值并返回 suspend_always(产一个挂一个 = 惰性)。
//       yield_value 也可返回自定义 awaiter(过滤/转换/不挂起)。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// 标准惰性 yield: 存值 + suspend_always
// ---------------------------------------------------------------------------
template <typename T>
struct YieldGen {
    struct promise_type {
        T current{};
        int yield_count = 0;

        YieldGen get_return_object() { return YieldGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(T v) {
            current = std::move(v);
            ++yield_count;
            return {};  // 总是挂起 → 调用者必须 resume 才继续
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit YieldGen(Handle h) noexcept : h_(h) {}
    ~YieldGen() {
        if (h_) {
            h_.destroy();
        }
    }
    YieldGen(YieldGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    YieldGen(const YieldGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().current; }
    int yields() const { return h_.promise().yield_count; }

private:
    Handle h_{};
};

// ---------------------------------------------------------------------------
// 自定义 yield_value: 偶数才真正挂起产出, 奇数直接吞掉(suspend_never)
// ---------------------------------------------------------------------------
struct FilterGen {
    struct promise_type {
        int current = 0;
        std::vector<int> produced;

        FilterGen get_return_object() { return FilterGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        // 返回类型可以是任意 awaitable。
        // 注意: 要给调用者看到的值必须在【挂起前】写入 promise
        // (典型写法在 yield_value 函数体里赋值, 再返回 suspend_always)。
        auto yield_value(int v) {
            struct Decide {
                bool take;
                bool await_ready() const noexcept { return !take; }  // 奇数: 不挂起
                void await_suspend(std::coroutine_handle<>) const noexcept {}
                void await_resume() const noexcept {}
            };
            if (v % 2 == 0) {
                current = v;
                produced.push_back(v);
                return Decide{true};  // 偶数: 挂起, 值已可见
            }
            return Decide{false};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit FilterGen(Handle h) noexcept : h_(h) {}
    ~FilterGen() {
        if (h_) {
            h_.destroy();
        }
    }
    FilterGen(FilterGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    FilterGen(const FilterGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    int value() const { return h_.promise().current; }
    const std::vector<int>& all() const { return h_.promise().produced; }

private:
    Handle h_{};
};

YieldGen<std::string> words() {
    co_yield "co_yield";
    co_yield "stores";
    co_yield "value";
}

FilterGen even_only() {
    for (int i = 1; i <= 6; ++i) {
        co_yield i;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== yield_value ===\n";

    auto g = words();
    assert(g.next() && g.value() == "co_yield");
    assert(g.next() && g.value() == "stores");
    assert(g.next() && g.value() == "value");
    assert(!g.next());
    assert(g.yields() == 3);
    std::cout << "  lazy yield_value count = " << g.yields() << '\n';

    auto f = even_only();
    // 每次 next 会连续吞掉奇数, 停在偶数
    assert(f.next() && f.value() == 2);
    assert(f.next() && f.value() == 4);
    assert(f.next() && f.value() == 6);
    assert(!f.next());
    assert((f.all() == std::vector<int>{2, 4, 6}));
    std::cout << "  custom yield_value filter: 2 4 6\n";

    std::cout << "yield_value: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/yield_value", run>;

}  // namespace
