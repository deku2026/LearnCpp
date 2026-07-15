// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B2 P2718)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : range_for_temporary_extension_cpp23
// Topic id : part6/b/section02/range_for_temporary_extension_cpp23
//
// 要点: C++23 P2718 — range-for 范围表达式中的临时延长到整个循环。
// 参考: https://en.cppreference.com/w/cpp/language/range-for

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

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B2 C++23 P2718 range-for temporary extension ===\n";

    assert(Holder::live == 0);

    // C++23：make_holder() 临时在整个 for 期间存活
    int sum = 0;
    for (int x : make_holder().items()) {
        assert(Holder::live >= 1);  // 父临时仍活
        sum += x;
    }
    assert(sum == 60);
    assert(Holder::live == 0);  // 循环结束后销毁

    // 直接临时 vector 同样安全
    sum = 0;
    for (int x : std::vector<int>{1, 2, 3, 4}) {
        sum += x;
    }
    assert(sum == 10);

    // 返回临时 string 的 range-for
    auto make_word = []() { return std::string("abcd"); };
    int chars = 0;
    for (char c : make_word()) {
        ++chars;
        (void)c;
    }
    assert(chars == 4);

    std::cout << "  P2718 extends all temporaries in range expression\n";
    std::cout << "range_for_temporary_extension_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/range_for_temporary_extension_cpp23", run>;

}  // namespace
