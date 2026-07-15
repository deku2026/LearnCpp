// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2.4 P2718 / B12 验收)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : range_for_temporary_extension_cpp23
// Topic id : part6/b/section02/range_for_temporary_extension_cpp23
//
// 要点: C++23 P2718 — range-for 范围表达式中的临时延长到整个循环。
// 验收: 讲清 P2718 修了哪种、没修哪些（未修见 gaps_not_yet_fixed）。
// 参考: https://en.cppreference.com/w/cpp/language/range-for  P2718R0

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Holder {
    std::vector<int> items_;
    static int live;
    explicit Holder(std::vector<int> v) : items_(std::move(v)) { ++live; }
    Holder(const Holder& o) : items_(o.items_) { ++live; }
    Holder(Holder&& o) noexcept : items_(std::move(o.items_)) { ++live; }
    ~Holder() { --live; }
    const std::vector<int>& items() const { return items_; }
};
int Holder::live = 0;

Holder make_holder() {
    return Holder{std::vector<int>{10, 20, 30}};
}

struct Nested {
    Holder inner;
    Nested() : inner(std::vector<int>{1, 2, 3, 4}) {}
    const Holder& get() const { return inner; }
};

Nested make_nested() {
    return Nested{};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 C++23 P2718 range-for temporary extension ===\n";

    assert(Holder::live == 0);

    // --- 入门: C++23 下父临时在整个 for 期间存活 ---
    [[maybe_unused]] int sum = 0;
    for (int x : make_holder().items()) {
        assert(Holder::live >= 1);
        sum += x;
    }
    assert(sum == 60);
    assert(Holder::live == 0);

    // 直接临时 vector
    sum = 0;
    for (int x : std::vector<int>{1, 2, 3, 4}) sum += x;
    assert(sum == 10);

    // --- 进阶: 返回临时 string 的 range-for ---
    auto make_word = []() { return std::string("abcd"); };
    [[maybe_unused]] int chars = 0;
    for (char c : make_word()) {
        ++chars;
        (void)c;
    }
    assert(chars == 4);

    // 更深嵌套：make_nested().get().items()
    sum = 0;
    for (int x : make_nested().get().items()) sum += x;
    assert(sum == 10);
    assert(Holder::live == 0);

    // --- 专家: 与 C++20 对照（概念）---
    // C++20 展开近似: auto&& __range = make_holder().items();
    //   → 只延长 items 的引用绑定，Holder 临时在语句末销毁 → 悬垂
    // C++23 P2718: 范围表达式里创建的所有临时延长到循环结束
    std::cout << "  P2718: all temporaries in range-for expression outlive loop\n";
    std::cout << "  FIXED: for (x : getTemp().member) patterns\n";
    std::cout << "  NOT fixed: split auto&& r = getTemp().member; for (x : r)\n";
    std::cout << "range_for_temporary_extension_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/range_for_temporary_extension_cpp23", run>;

}  // namespace
