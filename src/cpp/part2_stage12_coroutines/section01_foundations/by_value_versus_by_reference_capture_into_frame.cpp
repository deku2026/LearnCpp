// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : by_value_versus_by_reference_capture_into_frame
// Topic id : part2/stage12/section01/by_value_versus_by_reference_capture_into_frame
//
// 要点: 参数全部【拷贝/移动】进协程帧; 若参数是引用, 进帧的是引用本身,
//       被引对象仍在原处——挂起后原对象先销毁 → 悬垂(详见 section07)。
// 本文件只做【安全演示】+ 注释标明危险路径, 不制造 UB。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   [dcl.fct.def.coroutine] parameter copy into frame

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

template <typename T>
struct SafeGen {
    struct promise_type {
        T current{};
        SafeGen get_return_object() { return SafeGen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = std::move(v);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
    using Handle = std::coroutine_handle<promise_type>;
    explicit SafeGen(Handle h) noexcept : h_(h) {}
    ~SafeGen() {
        if (h_) {
            h_.destroy();
        }
    }
    SafeGen(SafeGen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    SafeGen(const SafeGen&) = delete;

    bool next() {
        h_.resume();
        return !h_.done();
    }
    const T& value() const { return h_.promise().current; }

private:
    Handle h_{};
};

// ✅ 按值: string 真正拷/移进帧, 与调用方生命周期解耦。
SafeGen<char> chars_by_value(std::string s) {
    for (char c : s) {
        co_yield c;
    }
}

// 引用参数: 进帧的是引用。仅在被引对象活过整个协程时安全。
SafeGen<char> chars_by_ref(const std::string& s) {
    for (char c : s) {
        co_yield c;
    }
}

// 按值容器: 元素数据在帧内副本上。
SafeGen<int> doubles(std::vector<int> xs) {
    for (int x : xs) {
        co_yield x * 2;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== by_value_versus_by_reference_capture_into_frame ===\n";

    // --- 按值: 即使源字符串销毁, 帧内副本仍可用 ---
    SafeGen<char> g1 = [&] {
        std::string tmp = "hi";
        return chars_by_value(std::move(tmp));  // 移进帧
    }();
    // 此处 tmp 已析构, 但帧里有独立 string
    assert(g1.next() && g1.value() == 'h');
    assert(g1.next() && g1.value() == 'i');
    assert(!g1.next());
    std::cout << "  by-value capture: safe after source destroyed\n";

    // --- 引用: 仅当具名对象活得够久 ---
    std::string live = "ok";
    auto g2 = chars_by_ref(live);
    assert(g2.next() && g2.value() == 'o');
    assert(g2.next() && g2.value() == 'k');
    assert(!g2.next());
    std::cout << "  by-ref + long-lived object: OK\n";

    // ⚠️ 危险(故意不执行, 避免 UB):
    //   for (char c : chars_by_ref(std::string("tmp"))) { ... }
    //   临时 string 在完整表达式结束后析构, 惰性 resume 时 s 已悬垂。
    // 纪律: 协程参数优先按值; 引用必须保证 lifetime ⊇ 协程。

    auto g3 = doubles({1, 2, 3});
    assert(g3.next() && g3.value() == 2);
    assert(g3.next() && g3.value() == 4);
    assert(g3.next() && g3.value() == 6);
    assert(!g3.next());
    std::cout << "  by-value vector in frame: 2 4 6\n";

    std::cout << "by_value_versus_by_reference_capture_into_frame: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage12/section01/by_value_versus_by_reference_capture_into_frame", run>;

}  // namespace
