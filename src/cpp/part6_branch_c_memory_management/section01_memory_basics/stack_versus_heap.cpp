// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C1 栈 vs 堆)
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : stack_versus_heap
// Topic id : part6/c/section01/stack_versus_heap
//
// 要点: 对象存哪取决于创建方式，不是「值类型一定在栈」。
// 参考: [basic.stc]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

namespace {

struct Widget {
    int x = 0;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C1 stack versus heap ===\n";

    Widget a;  // 自动存储 → 通常栈
    assert(a.x == 0);

    auto b = std::make_unique<Widget>();  // 动态 → 堆
    b->x = 1;
    assert(b->x == 1);

    static Widget c;  // 静态存储期
    c.x = 2;
    assert(c.x == 2);

    // vector 对象可在栈，元素在堆
    std::vector<Widget> v(3);
    v[0].x = 7;
    assert(v.size() == 3);
    assert(v[0].x == 7);

    // 数组成员跟随外层对象存储
    struct Big {
        Widget members[4];
    };
    Big stack_big{};
    auto heap_big = std::make_unique<Big>();
    stack_big.members[0].x = 3;
    heap_big->members[0].x = 4;
    assert(stack_big.members[0].x == 3);
    assert(heap_big->members[0].x == 4);

    std::cout << "  same type Widget can live on stack/heap/static\n";
    std::cout << "stack_versus_heap: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/stack_versus_heap", run>;

}  // namespace
