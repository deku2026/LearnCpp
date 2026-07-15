// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section04_coroutine_handle
// Item     : coroutine_handle_basics
// Topic id : part2/stage12/section04/coroutine_handle_basics
//
// 要点: coroutine_handle<Promise> 是【非拥有】句柄(像裸指针);
//   from_promise / address / from_address / 类型擦除到 handle<>
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/coroutine_handle

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

namespace {

struct Box {
    struct promise_type {
        int secret = 0;
        Box get_return_object() { return Box{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
        std::suspend_always yield_value(int v) {
            secret = v;
            return {};
        }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Box(Handle h) noexcept : h_(h) {}
    ~Box() {
        if (h_) {
            h_.destroy();
        }
    }
    Box(Box&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Box(const Box&) = delete;

    Handle typed() const noexcept { return h_; }

private:
    Handle h_{};
};

Box demo() {
    co_yield 123;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== coroutine_handle_basics ===\n";

    auto box = demo();
    auto h = box.typed();

    // 空/非空
    assert(static_cast<bool>(h));
    assert(h.address() != nullptr);

    // from_promise 往返
    auto& p = h.promise();
    [[maybe_unused]] auto h2 = std::coroutine_handle<Box::promise_type>::from_promise(p);
    assert(h2.address() == h.address());

    // 类型擦除
    [[maybe_unused]] std::coroutine_handle<> generic = h;
    assert(generic.address() == h.address());
    // generic.promise(); // 不可: void promise 无类型化 promise()

    // address / from_address(可跨 API 传递 void*)
    void* raw = h.address();
    [[maybe_unused]] auto h3 = std::coroutine_handle<Box::promise_type>::from_address(raw);
    assert(h3.address() == h.address());

    h.resume();
    assert(h.promise().secret == 123);
    assert(!h.done());
    h.resume();
    assert(h.done());

    std::cout << "  handle address=" << raw << " secret=" << h.promise().secret << '\n';
    std::cout << "  note: handle does NOT own the frame — Box dtor destroys\n";

    // ⚠️ 默认构造的 handle 比较像空指针, resume/destroy 空 handle → UB
    [[maybe_unused]] std::coroutine_handle<> empty{};
    assert(!empty);

    std::cout << "coroutine_handle_basics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section04/coroutine_handle_basics", run>;

}  // namespace
