// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 span)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_span
// Topic id : part6/b/section03/dangling_span
//
// 要点: span 是连续序列的非拥有视图；容器销毁/扩容后 span 悬垂。
// 参考: https://en.cppreference.com/w/cpp/container/span

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

namespace {

// ❌ std::span<int> bad() { std::vector<int> v{1,2}; return v; }
// ✅ 按值返回容器，或由调用方提供缓冲

int sum(std::span<const int> s) {
    return std::accumulate(s.begin(), s.end(), 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling span (safe contrasts) ===\n";

    std::vector<int> v{1, 2, 3, 4};
    std::span<int> sp = v;
    assert(sp.size() == 4);
    assert(sum(sp) == 10);
    sp[0] = 10;
    assert(v[0] == 10);

    // 扩容会使 span 失效——之后重建 span
    v.reserve(v.capacity() + 16);
    v.push_back(5);
    sp = v;  // 重新绑定
    assert(sp.size() == 5);
    assert(sum(sp) == 24);

    // 栈数组：span 与数组同寿
    {
        int buf[] = {7, 8, 9};
        std::span<int> local{buf};
        assert(sum(local) == 24);
    }

    std::array<int, 3> a{1, 1, 1};
    assert(sum(a) == 3);

    // 子 span 仍依赖底层
    std::span<int> mid = std::span<int>{v}.subspan(1, 2);
    assert(mid.size() == 2);
    assert(mid[0] == v[1]);

    std::cout << "  span does not extend underlying container lifetime\n";
    std::cout << "dangling_span: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_span", run>;

}  // namespace
