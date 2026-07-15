// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : destructor_cannot_be_a_coroutine
// Topic id : part2/stage12/section07/destructor_cannot_be_a_coroutine
//
// 要点: 构造/析构/main/constexpr/可变参数函数不能是协程
//       (不得含 co_await/co_yield/co_return)。
//       异步清理 → 单独 Task + 同步汇合, 而非 ~T() 内 co_await。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   [dcl.fct.def.coroutine] restrictions

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <optional>
#include <utility>

namespace {

template <typename T>
struct Task {
    struct promise_type {
        std::optional<T> result;
        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T v) { result = std::move(v); }
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit Task(Handle h) noexcept : h_(h) {}
    ~Task() {
        if (h_) {
            h_.destroy();
        }
    }
    Task(Task&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Task(const Task&) = delete;

    T get() {
        h_.resume();
        assert(h_.done());
        return std::move(*h_.promise().result);
    }

private:
    Handle h_{};
};

// ✅ 普通函数可以是协程工厂
Task<int> async_close_resource(int id) {
    // 模拟异步清理步骤
    co_return id;
}

struct Resource {
    int id;
    bool cleaned = false;

    explicit Resource(int i) : id(i) {}

    // ❌ 不能写:  ~Resource() { co_await something; }
    // 析构必须同步完成对象生命周期义务。
    ~Resource() {
        // 若必须做“异步风格”清理, 在外部先 await 再销毁对象:
        // 这里仅同步标记, 证明 dtor 是普通函数。
        if (!cleaned) {
            cleaned = true;  // 同步兜底
        }
    }

    // 显式异步清理 API(协程), 由调用者在 dtor 前 co_await/get
    Task<int> async_cleanup() {
        cleaned = true;
        co_return id;
    }
};

// 构造函数同样不能是协程:
//   Resource::Resource() { co_await init(); }  // 非法
Task<Resource> async_open(int id) {
    // 工厂协程: 异步初始化后返回对象
    co_return Resource{id};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== destructor_cannot_be_a_coroutine ===\n";

    {
        auto opened = async_open(7);
        Resource r = opened.get();
        assert(r.id == 7);

        // 先异步清理, 再让 dtor 跑同步兜底
        auto done = r.async_cleanup();
        assert(done.get() == 7);
        assert(r.cleaned);
        std::cout << "  cleanup via Task before destroy: id=7\n";
    }

    {
        auto t = async_close_resource(3);
        assert(t.get() == 3);
        std::cout << "  free function coroutine OK; dtor/ctor/main cannot\n";
    }

    // 禁止列表(语言规则): constructors, destructors, main,
    // constexpr/consteval functions, varargs functions, ...

    std::cout << "destructor_cannot_be_a_coroutine: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/destructor_cannot_be_a_coroutine", run>;

}  // namespace
