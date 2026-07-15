// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : lifetime_management
// Topic id : part2/stage12/section07/lifetime_management
//
// 要点: handle 非拥有 → 必须明确帧所有权(RAII destroy);
//       避免双重 destroy / 泄漏 / 运行中 destroy / 用悬垂 handle。
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/coroutine_handle
//   文档步骤 12.4

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

namespace {

struct Owning {
    struct promise_type {
        int v = 0;
        Owning get_return_object() { return Owning{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int x) {
            v = x;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;

    explicit Owning(Handle h) noexcept : h_(h) {}
    ~Owning() {
        if (h_) {
            h_.destroy();  // 唯一 destroy 点
        }
    }
    Owning(const Owning&) = delete;
    Owning& operator=(const Owning&) = delete;
    Owning(Owning&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Owning& operator=(Owning&& o) noexcept {
        if (this != &o) {
            if (h_) {
                h_.destroy();
            }
            h_ = std::exchange(o.h_, {});
        }
        return *this;
    }

    bool next() {
        h_.resume();
        return !h_.done();
    }
    int value() const { return h_.promise().v; }

    // 移交原始 handle 时必须清空自身, 防止双重 destroy
    Handle release() noexcept { return std::exchange(h_, {}); }

private:
    Handle h_{};
};

Owning seq() {
    co_yield 1;
    co_yield 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== lifetime_management ===\n";

    // 1) RAII 拥有
    {
        auto g = seq();
        assert(g.next() && g.value() == 1);
        assert(g.next() && g.value() == 2);
        assert(!g.next());
        std::cout << "  RAII destroy on scope exit\n";
    }

    // 2) move 后源为空, 不 double-destroy
    {
        auto a = seq();
        auto b = std::move(a);
        assert(b.next() && b.value() == 1);
        // a 已空, 析构 no-op
        std::cout << "  move transfers ownership\n";
    }

    // 3) release 后由接手方 destroy
    {
        auto g = seq();
        auto h = g.release();
        h.resume();
        assert(h.promise().v == 1);
        h.destroy();  // 接手方负责
        std::cout << "  release + explicit destroy\n";
    }

    // ⚠️ 反模式(不执行):
    //   - 拷贝 handle 两边都 destroy
    //   - Task 未 await 就丢弃(可能泄漏或未跑完 — 视设计)
    //   - 协程运行中 destroy
    //   - destroy 后再 resume

    std::cout << "lifetime_management: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/lifetime_management", run>;

}  // namespace
