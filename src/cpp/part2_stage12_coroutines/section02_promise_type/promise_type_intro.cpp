// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section02_promise_type
// Item     : promise_type_intro
// Topic id : part2/stage12/section02/promise_type_intro
//
// 要点: promise_type = 协程控制器; 编译器经 coroutine_traits<R>::promise_type
//       查找(默认 R::promise_type)。钩子对应改写伪代码里的每一次调用。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   https://en.cppreference.com/w/cpp/coroutine/coroutine_traits
//   Lewis Baker: Understanding the promise type

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// 完整最小 promise 钩子一览(生成器风格)。
template <typename T>
class IntroGen {
public:
    struct promise_type {
        T value_{};
        std::vector<std::string> log;  // 教学: 记录钩子调用顺序

        void note(const char* s) { log.emplace_back(s); }

        IntroGen get_return_object() {
            note("get_return_object");
            return IntroGen{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept {
            note("initial_suspend");
            return {};
        }
        std::suspend_always final_suspend() noexcept {
            note("final_suspend");
            return {};
        }
        std::suspend_always yield_value(T v) {
            note("yield_value");
            value_ = std::move(v);
            return {};
        }
        void return_void() { note("return_void"); }
        void unhandled_exception() {
            note("unhandled_exception");
            std::terminate();
        }
        // 可选: await_transform / operator new — 见后续 topic
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit IntroGen(Handle h) noexcept : h_(h) {}
    ~IntroGen() {
        if (h_) {
            h_.destroy();
        }
    }
    IntroGen(IntroGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    IntroGen(const IntroGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().value_; }
    const std::vector<std::string>& hook_log() const { return h_.promise().log; }

private:
    Handle h_{};
};

IntroGen<int> demo() {
    co_yield 7;
    co_yield 8;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== promise_type_intro ===\n";

    auto g = demo();
    // ramp: get_return_object → initial_suspend(挂起, 懒启动)
    // 第一次 resume → 跑到 co_yield → yield_value → suspend
    assert(g.next() && g.value() == 7);
    assert(g.next() && g.value() == 8);
    assert(!g.next());  // return_void → final_suspend

    const auto& log = g.hook_log();
    std::cout << "  hook order:\n";
    for (const auto& s : log) {
        std::cout << "    " << s << '\n';
    }

    // 期望顺序(懒启动 generator):
    // get_return_object, initial_suspend,
    // yield_value, yield_value, return_void, final_suspend
    assert(log.size() >= 6);
    assert(log[0] == "get_return_object");
    assert(log[1] == "initial_suspend");
    assert(log[2] == "yield_value");
    assert(log[3] == "yield_value");
    assert(log[4] == "return_void");
    assert(log[5] == "final_suspend");

    // 换 promise 策略 → 同一语法变成 Task / 其它类型(见 section06)。
    std::cout << "promise_type_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section02/promise_type_intro", run>;

}  // namespace
