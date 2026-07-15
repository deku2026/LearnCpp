// LearnCpp topic
// Doc      : 第6部分-支线G · UAF
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : use_after_free_deep
// Topic id : part6/g/section03/use_after_free_deep
//
// 要点: 释放后使用 = UB; RAII/智能指针消除大多数 UAF。
// 不触发 UB。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

struct Node {
    int id;
    explicit Node(int i) : id(i) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G use-after-free (safe patterns) ===\n";

    // 危险模式 (不运行): int* p = new int(1); delete p; *p;
    {
        auto p = std::make_unique<Node>(7);
        assert(p->id == 7);
    }  // 自动释放

    std::shared_ptr<Node> a = std::make_shared<Node>(1);
    std::weak_ptr<Node> w = a;
    {
        auto locked = w.lock();
        assert(locked);
        assert(locked->id == 1);
    }
    a.reset();
    assert(w.expired());
    assert(!w.lock());  // 安全: 不解引用悬空

    // 迭代器失效相关: 先保留值再修改容器
    std::vector<int> v{1, 2, 3};
    int first = v.front();
    v.push_back(4);  // 可能重分配
    assert(first == 1);

    std::cout << "  ASan: detect-heap-use-after-free\n";
    std::cout << "use_after_free_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/use_after_free_deep", run>;

}  // namespace
