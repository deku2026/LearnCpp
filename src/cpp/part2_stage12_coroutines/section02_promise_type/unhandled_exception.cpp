// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : unhandled_exception
// Topic id : part2/stage12/section02/unhandled_exception
//
// 要点: 函数体抛异常 → promise.unhandled_exception(); 通常 current_exception() 存起,
//       在 resume/await_resume/迭代时 rethrow。异常后仍会走到 final_suspend。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   https://en.cppreference.com/w/cpp/error/exception_ptr

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

template <typename T>
struct ExGen {
    struct promise_type {
        T value_{};
        std::exception_ptr ep_{};

        ExGen get_return_object() { return ExGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            value_ = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { ep_ = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit ExGen(Handle h) noexcept : h_(h) {}
    ~ExGen() {
        if (h_) {
            h_.destroy();
        }
    }
    ExGen(ExGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    ExGen(const ExGen&) = delete;

    // 返回: true=有值, false=正常结束; 异常则抛出
    bool next() {
        h_.resume();
        if (h_.promise().ep_) {
            std::rethrow_exception(h_.promise().ep_);
        }
        return !h_.done();
    }
    const T& value() const { return h_.promise().value_; }
    bool done() const noexcept { return h_.done(); }

private:
    Handle h_{};
};

ExGen<int> good_then_throw() {
    co_yield 1;
    co_yield 2;
    throw std::runtime_error("boom-from-coroutine-body");
    co_yield 3;  // 不可达
}

ExGen<int> clean() {
    co_yield 10;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== unhandled_exception ===\n";

    {
        auto g = clean();
        assert(g.next() && g.value() == 10);
        assert(!g.next());
        std::cout << "  clean path: no exception\n";
    }

    {
        auto g = good_then_throw();
        assert(g.next() && g.value() == 1);
        assert(g.next() && g.value() == 2);

        [[maybe_unused]] bool caught = false;
        try {
            (void)g.next();  // body 抛异常 → unhandled_exception 存 ep → 这里重抛
        } catch (const std::runtime_error& e) {
            caught = true;
            std::cout << "  rethrown: " << e.what() << '\n';
            assert(std::string(e.what()) == "boom-from-coroutine-body");
        }
        assert(caught);
        // 异常处理后协程进入 final_suspend → done
        assert(g.done());
    }

    // ⚠️ 若 unhandled_exception 里再抛 / terminate, 会直接崩 —— 教学上优先存 ep。
    std::cout << "unhandled_exception: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/unhandled_exception", run>;

}  // namespace
