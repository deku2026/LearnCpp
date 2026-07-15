// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : get_return_object
// Topic id : part2/stage12/section02/get_return_object
//
// 要点: get_return_object() 在 initial_suspend 之前调用, 产生返回给调用者的对象;
//       通常用 coroutine_handle::from_promise(*this) 包装句柄(RAII 拥有帧)。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   https://en.cppreference.com/w/cpp/coroutine/coroutine_handle

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

namespace {

struct GroBox {
    struct promise_type {
        int cookie = 0;

        // ① 必须返回“协程函数的返回类型”
        GroBox get_return_object() {
            // 从 promise 反查 handle —— 帧已存在, promise 在帧内
            auto h = std::coroutine_handle<promise_type>::from_promise(*this);
            cookie = 0xC0FFEE;
            std::cout << "  get_return_object: handle=" << h.address() << " cookie set\n";
            return GroBox{h};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit GroBox(Handle h) noexcept : h_(h) {}
    ~GroBox() {
        if (h_) {
            h_.destroy();
        }
    }
    GroBox(GroBox&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    GroBox(const GroBox&) = delete;

    void run_body() { h_.resume(); }
    int cookie() const { return h_.promise().cookie; }
    void* addr() const noexcept { return h_.address(); }
    bool done() const noexcept { return h_.done(); }

private:
    Handle h_{};
};

// 返回类型 = GroBox → 编译器调 GroBox::promise_type::get_return_object()
GroBox make_box() {
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== get_return_object ===\n";

    GroBox box = make_box();
    // 调用者拿到的就是 get_return_object 的结果
    assert(box.addr() != nullptr);
    assert(box.cookie() == 0xC0FFEE);
    assert(!box.done());  // initial_suspend 后仍挂起

    box.run_body();  // 跑完 body → final_suspend
    assert(box.done());
    std::cout << "  after resume: done, cookie still " << box.cookie() << '\n';

    // 注意: get_return_object 可返回非 promise 所属类型(经转换),
    // 但教学上几乎总是“包装 handle 的 RAII 对象”。

    std::cout << "get_return_object: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/get_return_object", run>;

}  // namespace
