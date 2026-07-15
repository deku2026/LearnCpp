// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 悬垂指针)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_pointer
// Topic id : part6/b/section03/dangling_pointer
//
// 要点: delete/离开作用域后指针悬垂。安全：智能指针、置空、缩短借用窗口。
// ASan: heap-use-after-free / stack-use-after-scope
// 参考: [basic.life]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling pointer (safe contrasts) ===\n";

    // --- 入门: 独占所有权 ---
    {
        auto owner = std::make_unique<int>(42);
        [[maybe_unused]] int* borrow = owner.get();
        assert(*borrow == 42);
        owner.reset();
        // *borrow;  // ❌ UAF — ASan: heap-use-after-free
        borrow = nullptr;
        assert(borrow == nullptr);
    }

    // --- 进阶: 栈对象借用窗口 ---
    {
        int x = 5;
        [[maybe_unused]] int* p = &x;
        assert(*p == 5);
        p = nullptr;
    }
    // int* escaped; { int y=1; escaped=&y; } *escaped; // ❌ stack-use-after-scope

    {
        int* p = new int(7);
        assert(*p == 7);
        delete p;
        p = nullptr;
    }

    // weak_ptr 检测存活（共享所有权侧）
    {
        std::weak_ptr<int> w;
        {
            auto s = std::make_shared<int>(9);
            w = s;
            assert(!w.expired());
            if (auto locked = w.lock()) assert(*locked == 9);
        }
        assert(w.expired());
        assert(w.lock() == nullptr);
    }

    // --- 专家: 置空是纪律不是类型系统 ---
    // raw 指针无法在类型上表达「已失效」；prefer unique_ptr + span/T& 借用
    std::cout << "  after free/reset: null the borrow; prefer unique_ptr\n";
    std::cout << "  unified model: pointer = non-owning view of someone else's object\n";
    std::cout << "dangling_pointer: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_pointer", run>;

}  // namespace
