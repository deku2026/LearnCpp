// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 悬垂指针)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_pointer
// Topic id : part6/b/section03/dangling_pointer
//
// 要点: delete/离开作用域后指针悬垂。安全：智能指针、置空、缩短借用窗口。
// 参考: ASan heap-use-after-free / stack-use-after-scope

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling pointer (safe contrasts) ===\n";

    // ✅ 独占所有权
    {
        auto owner = std::make_unique<int>(42);
        int* borrow = owner.get();
        assert(*borrow == 42);
        owner.reset();
        // *borrow;  // ❌ UAF
        borrow = nullptr;  // 防御性清空
        assert(borrow == nullptr);
    }

    // ✅ 作用域内借用栈对象
    {
        int x = 5;
        int* p = &x;
        assert(*p == 5);
        p = nullptr;  // 离开前清空观察指针
    }
    // int* escaped; { int y=1; escaped=&y; } *escaped; // ❌

    // ✅ new/delete 配对后不使用
    {
        int* p = new int(7);
        assert(*p == 7);
        delete p;
        p = nullptr;
        assert(p == nullptr);
    }

    // 共享：weak_ptr 检测存活
    {
        std::weak_ptr<int> w;
        {
            auto s = std::make_shared<int>(9);
            w = s;
            assert(!w.expired());
            if (auto locked = w.lock()) {
                assert(*locked == 9);
            }
        }
        assert(w.expired());
        assert(w.lock() == nullptr);
    }

    std::cout << "  after free/reset: null borrow; prefer unique_ptr\n";
    std::cout << "dangling_pointer: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_pointer", run>;

}  // namespace
