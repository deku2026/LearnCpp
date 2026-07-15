// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3.1 悬垂引用 / 统一模型)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_reference
// Topic id : part6/b/section03/dangling_reference
//
// 要点: 悬垂 = 借用指向已死对象。安全：按值返回 / 借用方拥有更长寿。
// 验收: 五种+悬垂形态同一类风险（本文件是引用形态）。
// 参考: [basic.life] Core Guidelines Lifetime

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

// ❌ const std::string& bad() { std::string local="x"; return local; }
// ✅ 按值返回
std::string good_value() {
    std::string local = "owned";
    return local;
}

// ✅ 返回调用方拥有容器上的引用
const std::string& at(const std::vector<std::string>& v, std::size_t i) {
    return v.at(i);
}

// 成员存引用：调用方保证被引用对象 outlive
struct View {
    const std::string& s;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling reference (safe contrasts) ===\n";

    // --- 入门 ---
    std::string s = good_value();
    assert(s == "owned");

    std::vector<std::string> names{"alpha", "beta"};
    [[maybe_unused]] const std::string& r = at(names, 1);
    assert(r == "beta");
    // names 仍存活 → r 安全

    {
        std::string long_lived = "payload";
        [[maybe_unused]] View v{long_lived};
        assert(v.s == "payload");
    }
    // View 不得比 long_lived 活得更久

    // --- 进阶: 容器失效使引用失效 ---
    {
        std::vector<int> v{1, 2};
        // int& first = v[0]; v.reserve(...); 使用 first → 可能 UB
        v.reserve(v.capacity() + 8);
        v.push_back(3);
        assert(v[0] == 1 && v.back() == 3);  // 下标重新取得
    }

    // 绑临时的 const& 延长（安全）vs 经函数不延长（危险，注释）
    {
        const std::string& ext = std::string("extend");
        assert(ext == "extend");
    }
    // const std::string& d = at(std::vector<std::string>{"x"}, 0); // 悬垂

    // --- 专家: 统一模型一句话 ---
    std::cout << "  reference is a borrow; owner must outlive the borrow\n";
    std::cout << "  same risk class as pointer/iterator/string_view/span/coro-ref\n";
    std::cout << "  never return ref/pointer to function-local automatic\n";
    std::cout << "dangling_reference: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_reference", run>;

}  // namespace
