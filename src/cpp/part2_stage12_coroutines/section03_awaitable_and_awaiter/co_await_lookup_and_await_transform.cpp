// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section03_awaitable_and_awaiter
// Item     : co_await_lookup_and_await_transform
// Topic id : part2/stage12/section03/co_await_lookup_and_await_transform
//
// 要点: co_await expr 两步:
//   1) expr → awaitable: 若有 promise.await_transform 则拦截
//   2) awaitable → awaiter: member/ADL operator co_await, 否则自身即 awaiter
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   Lewis Baker: Understanding operator co_await

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

std::vector<std::string> g_trace;

// 裸 awaiter(自身即可 co_await)
struct RawAwaiter {
    int v;
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const {
        g_trace.push_back("RawAwaiter.resume");
        return v;
    }
};

// 仅 awaitable: 通过 member operator co_await 变成 awaiter
struct WrappedAwaitable {
    int v;
    auto operator co_await() const {
        g_trace.push_back("WrappedAwaitable.operator co_await");
        struct Aw {
            int v;
            bool await_ready() const noexcept { return true; }
            void await_suspend(std::coroutine_handle<>) const noexcept {}
            int await_resume() const {
                g_trace.push_back("Wrapped.inner.resume");
                return v + 1;
            }
        };
        return Aw{v};
    }
};

// 将被 await_transform 包装的标签类型
struct Tag {
    int v;
};

struct TransformTask {
    struct promise_type {
        int result = 0;

        TransformTask get_return_object() {
            return TransformTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { result = v; }
        void unhandled_exception() { std::terminate(); }

        // ⭐ 每个 co_await expr 先经此转换(initial/final/yield 除外)
        auto await_transform(Tag t) {
            g_trace.push_back("await_transform(Tag)");
            struct Aw {
                int v;
                bool await_ready() const noexcept { return true; }
                void await_suspend(std::coroutine_handle<>) const noexcept {}
                int await_resume() const {
                    g_trace.push_back("transformed.resume");
                    return v * 10;
                }
            };
            return Aw{t.v};
        }

        // 透传其它 awaitable, 否则只能 await Tag
        template <typename A>
        A&& await_transform(A&& a) noexcept {
            g_trace.push_back("await_transform(passthrough)");
            return std::forward<A>(a);
        }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit TransformTask(Handle h) noexcept : h_(h) {}
    ~TransformTask() {
        if (h_) {
            h_.destroy();
        }
    }
    TransformTask(TransformTask&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    TransformTask(const TransformTask&) = delete;

    int run() {
        h_.resume();
        assert(h_.done());
        return h_.promise().result;
    }

private:
    Handle h_{};
};

TransformTask demo() {
    int a = co_await RawAwaiter{2};        // passthrough → raw awaiter
    int b = co_await WrappedAwaitable{3};  // passthrough → operator co_await
    int c = co_await Tag{4};               // await_transform(Tag)
    co_return a + b + c;                   // 2 + (3+1) + 40 = 46
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== co_await_lookup_and_await_transform ===\n";
    g_trace.clear();

    auto t = demo();
    assert(t.run() == 46);

    for (const auto& s : g_trace) {
        std::cout << "    " << s << '\n';
    }

    // 查找顺序摘要:
    //   co_await Tag{4}
    //     → promise.await_transform(Tag) → Aw{4}
    //     → Aw 无 operator co_await → 自身为 awaiter
    //   co_await WrappedAwaitable
    //     → await_transform 透传
    //     → member operator co_await → 内层 awaiter

    std::cout << "co_await_lookup_and_await_transform: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section03/co_await_lookup_and_await_transform", run>;

}  // namespace
