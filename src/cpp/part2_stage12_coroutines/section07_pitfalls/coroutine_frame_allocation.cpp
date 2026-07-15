// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : coroutine_frame_allocation
// Topic id : part2/stage12/section07/coroutine_frame_allocation
//
// 要点: 帧默认 operator new 堆分配; promise 可自定义 new/delete(池化);
//       HALO 可能消除堆分配但不保证。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   P0981 HALO

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <new>
#include <utility>

namespace {

std::size_t g_new_count = 0;
std::size_t g_delete_count = 0;
std::size_t g_last_frame_bytes = 0;

struct Pooled {
    struct promise_type {
        int value = 0;

        // 自定义帧分配: 计数 + ::operator new
        static void* operator new(std::size_t n) {
            ++g_new_count;
            g_last_frame_bytes = n;
            std::cout << "    promise_type::operator new(" << n << ")\n";
            return ::operator new(n);
        }
        static void operator delete(void* p, std::size_t n) noexcept {
            ++g_delete_count;
            std::cout << "    promise_type::operator delete(" << n << ")\n";
            ::operator delete(p, n);
        }

        Pooled get_return_object() { return Pooled{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int v) {
            value = v;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit Pooled(Handle h) noexcept : h_(h) {}
    ~Pooled() {
        if (h_) {
            h_.destroy();
        }
    }
    Pooled(Pooled&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Pooled(const Pooled&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    int value() const { return h_.promise().value; }

private:
    Handle h_{};
};

Pooled make_seq(int n) {
    // 跨挂起局部增大帧体积(便于观察 n)
    char pad[64]{};
    for (int i = 0; i < n; ++i) {
        pad[i % 64] = static_cast<char>(i);
        co_yield i;
        (void)pad;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== coroutine_frame_allocation ===\n";
    g_new_count = g_delete_count = 0;

    {
        auto g = make_seq(3);
        assert(g_new_count >= 1);
        assert(g_last_frame_bytes > 0);
        assert(g.next() && g.value() == 0);
        assert(g.next() && g.value() == 1);
        assert(g.next() && g.value() == 2);
        assert(!g.next());
        std::cout << "  frame bytes≈" << g_last_frame_bytes << " news=" << g_new_count << '\n';
    }
    assert(g_delete_count == g_new_count);
    std::cout << "  destroy matched delete count=" << g_delete_count << '\n';

    // HALO: 若编译器证明帧不逃逸, 可能放调用者栈上, 跳过 new。
    // 本 topic 用自定义 operator new 强制可见分配(通常禁用/绕过部分 elision 可见性)。
    // 热路径: 池化 new、减少协程创建次数、测量优先。

    std::cout << "coroutine_frame_allocation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/coroutine_frame_allocation", run>;

}  // namespace
