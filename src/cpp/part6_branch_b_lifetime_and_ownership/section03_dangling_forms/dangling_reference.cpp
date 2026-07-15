// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 悬垂引用)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_reference
// Topic id : part6/b/section03/dangling_reference
//
// 要点: 引用是借用——被引用对象先死则悬垂。安全对照：按值返回 / 调用方拥有。
// 参考: [basic.life] Core Guidelines Lifetime

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

// ❌ 模式: const std::string& bad() { std::string local="x"; return local; }
// ✅ 按值返回
std::string good_value() {
    std::string local = "owned";
    return local;
}

// ✅ 返回调用方拥有数据上的引用
const std::string& at(const std::vector<std::string>& v, std::size_t i) {
    return v.at(i);
}

// 成员存引用：调用方必须保证被引对象更长寿
struct View {
    const std::string& s;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling reference (safe contrasts) ===\n";

    std::string s = good_value();
    assert(s == "owned");

    std::vector<std::string> names{"alpha", "beta"};
    const std::string& r = at(names, 1);
    assert(r == "beta");
    // names 仍存活 → r 安全

    {
        std::string long_lived = "payload";
        View v{long_lived};
        assert(v.s == "payload");
    }
    // View 已销毁；不要把 View 活得比 long_lived 更久

    // 容器 reallocation 会使引用失效——重新获取
    {
        std::vector<int> v{1, 2};
        v.reserve(v.capacity() + 8);
        v.push_back(3);
        // 不保留旧 &v[0]；用下标
        assert(v[0] == 1 && v.back() == 3);
    }

    std::cout << "  never return ref/pointer to function-local automatic\n";
    std::cout << "dangling_reference: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_reference", run>;

}  // namespace
