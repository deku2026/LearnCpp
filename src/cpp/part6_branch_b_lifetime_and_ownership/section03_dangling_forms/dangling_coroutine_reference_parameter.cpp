// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 协程引用参数)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_coroutine_reference_parameter
// Topic id : part6/b/section03/dangling_coroutine_reference_parameter
//
// 要点: 协程按引用接临时实参，挂起后实参可能已死。
// 安全: 按值传参，或保证 owner 跨 await 存活。
// 参考: 阶段12；[dcl.fct.def.coroutine]

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>

namespace {

struct Task {
    struct promise_type {
        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> h{};
    explicit Task(std::coroutine_handle<promise_type> handle) : h(handle) {}
    Task(Task&& o) noexcept : h(std::exchange(o.h, {})) {}
    Task(const Task&) = delete;
    ~Task() {
        if (h) h.destroy();
    }
    void resume() {
        if (h && !h.done()) h.resume();
    }
    bool done() const { return !h || h.done(); }
};

// ❌ Task bad(const std::string& s) { co_await ...; use(s); }
//    bad(std::string("tmp"))：临时在 full-expression 结束即销毁。

// ✅ 按值：副本进入协程帧
Task good_by_value(std::string s) {
    co_await std::suspend_always{};
    assert(s == "payload");
    co_return;
}

// ✅ 引用但调用方保证跨 await 存活
Task good_ref_with_owner(const std::string& s) {
    co_await std::suspend_always{};
    assert(s == "long-lived");
    co_return;
}

// 进阶: 按值整数同样安全（小对象直接进帧）
Task good_int(int x) {
    co_await std::suspend_always{};
    assert(x == 42);
    co_return;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 coroutine ref-parameter dangling (safe) ===\n";

    {
        Task t = good_by_value("payload");
        t.resume();  // initial_suspend
        t.resume();  // body after await
        while (!t.done()) t.resume();
    }

    {
        std::string owner = "long-lived";
        Task t = good_ref_with_owner(owner);
        t.resume();
        while (!t.done()) t.resume();
        assert(owner == "long-lived");
    }

    {
        Task t = good_int(42);
        t.resume();
        while (!t.done()) t.resume();
    }

    // --- 专家: 统一模型 ---
    // 协程引用参数 = 挂起后仍可能使用的「长寿命借用」
    // 临时实参寿命默认只到创建协程的完整表达式 → 与 string_view 返回局部同类风险
    std::cout << "  prefer by-value into coroutines; ref only if owner outlives awaits\n";
    std::cout << "  form #7 of unified dangling model (borrow outlives owner)\n";
    std::cout << "dangling_coroutine_reference_parameter: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_coroutine_reference_parameter", run>;

}  // namespace
