// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B5/B7 裸指针=借用)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : raw_pointer_as_borrow
// Topic id : part6/b/section04/raw_pointer_as_borrow
//
// 要点: 现代心智——裸 T* 默认表示「不拥有、可能为空」的借用。
// 参考: C++ Core Guidelines R.3 / F.7

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <span>
#include <string>

namespace {

// 拥有：工厂返回 unique_ptr
std::unique_ptr<std::string> make_widget(const char* s) {
    return std::make_unique<std::string>(s);
}

// 借用：裸指针 / 引用 —— 不 delete
void annotate(std::string* w) {
    if (!w) return;
    w->append("!");
}

void annotate_ref(std::string& w) {
    w.append("?");
}

// 数组借用：span 绑定长度
int sum(std::span<const int> s) {
    int t = 0;
    for (int x : s) t += x;
    return t;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B5/B7 raw pointer as borrow ===\n";

    auto owner = make_widget("hi");
    std::string* borrow = owner.get();  // 不拥有
    annotate(borrow);
    assert(*owner == "hi!");
    annotate_ref(*owner);
    assert(*owner == "hi!?");

    annotate(nullptr);  // 允许空：调用方需检查

    int arr[] = {1, 2, 3};
    assert(sum(arr) == 6);

    // 观察指针生命周期 ≤ owner
    {
        std::string* p = owner.get();
        assert(p && *p == "hi!?");
        owner.reset();
        p = nullptr;  // 不悬挂
    }

    std::cout << "  raw T* = non-owning maybe-null borrow by default\n";
    std::cout << "raw_pointer_as_borrow: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/raw_pointer_as_borrow", run>;

}  // namespace
