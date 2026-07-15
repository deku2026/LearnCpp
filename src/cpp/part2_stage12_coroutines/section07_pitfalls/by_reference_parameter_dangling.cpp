// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : by_reference_parameter_dangling
// Topic id : part2/stage12/section07/by_reference_parameter_dangling
//
// 要点: 引用参数进帧的是引用; 临时量在完整表达式后析构, 惰性 resume 时悬垂。
//       本文件【安全演示】对照: 按值 / 长寿命引用 OK; 危险路径只注释不执行。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   文档步骤 2.4 / 12.1

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#define LEARN_HAS_STD_GENERATOR 1
#else
#define LEARN_HAS_STD_GENERATOR 0
#endif

namespace {

#if !LEARN_HAS_STD_GENERATOR
template <typename T>
struct Gen {
    struct promise_type {
        T current{};
        Gen get_return_object() { return Gen{std::coroutine_handle<promise_type>::from_promise(*this)}; }
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
    explicit Gen(Handle h) noexcept : h_(h) {}
    ~Gen() {
        if (h_) {
            h_.destroy();
        }
    }
    Gen(Gen&& o) noexcept : h_(std::exchange(o.h_, {})) {}
    Gen(const Gen&) = delete;
    struct iterator {
        Handle h{};
        void operator++() { h.resume(); }
        const T& operator*() const { return h.promise().current; }
        bool operator==(std::default_sentinel_t) const { return !h || h.done(); }
    };
    iterator begin() {
        if (h_) {
            h_.resume();
        }
        return iterator{h_};
    }
    std::default_sentinel_t end() const noexcept { return {}; }

private:
    Handle h_{};
};
template <typename T>
using generator_t = Gen<T>;
#else
template <typename T>
using generator_t = std::generator<T>;
#endif

// ⚠️ 危险签名: 引用不拥有字符数据
generator_t<char> each_char_ref(const std::string& s) {
    for (char c : s) {
        co_yield c;
    }
}

// ✅ 安全: 按值, 字符串在帧内
generator_t<char> each_char_val(std::string s) {
    for (char c : s) {
        co_yield c;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== by_reference_parameter_dangling ===\n";

    // ✅ 具名对象活过整个 for
    {
        std::string live = "ok";
        std::string out;
        for (char c : each_char_ref(live)) {
            out.push_back(c);
        }
        assert(out == "ok");
        std::cout << "  ref + long-lived named object: OK\n";
    }

    // ✅ 按值: 临时也可以, 数据已进帧
    {
        std::string out;
        for (char c : each_char_val(std::string("hi"))) {
            out.push_back(c);
        }
        assert(out == "hi");
        std::cout << "  by-value + temporary: OK (copy/move into frame)\n";
    }

    // ⚠️ 危险路径 — 故意不执行, 避免 UB:
    //
    //   for (char c : each_char_ref(std::string("tmp"))) { use(c); }
    //
    // 时序:
    //   1) 构造临时 string("tmp")
    //   2) 调用 each_char_ref: 引用参数绑临时, 引用本身拷进协程帧
    //   3) 返回 generator(懒, 尚未用 s)
    //   4) 完整表达式结束 → 临时 string 析构
    //   5) range-for 首次 resume → 读 s → 悬垂 → UB
    //
    // 为何比普通函数更易踩: 普通函数立即用完引用; 协程挂起后延迟使用。

    std::cout << "  discipline: prefer by-value coroutine params\n";
    std::cout << "by_reference_parameter_dangling: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/by_reference_parameter_dangling", run>;

}  // namespace
