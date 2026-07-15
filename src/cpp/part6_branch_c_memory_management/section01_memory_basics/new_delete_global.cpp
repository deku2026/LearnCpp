// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C2 全局 new/delete)
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_global
// Topic id : part6/c/section01/new_delete_global
//
// 要点: new 表达式 = operator new 分配 + 构造；delete = 析构 + operator delete。
//       不重载全局 operator new（避免污染整个进程），用 ::operator new 直接调用演示。
// 参考: https://en.cppreference.com/w/cpp/memory/new/operator_new

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>
#include <string>

namespace {

struct Tracer {
    int v;
    explicit Tracer(int x) : v(x) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C2 global operator new / delete (via ::) ===\n";

    // 直接分配原始内存（不构造）
    void* raw = ::operator new(sizeof(Tracer));
    assert(raw != nullptr);

    // placement 构造
    Tracer* p = ::new (raw) Tracer{42};
    assert(p->v == 42);

    p->~Tracer();
    ::operator delete(raw);

    // new 表达式封装：分配 + 构造
    Tracer* q = new Tracer{7};
    assert(q->v == 7);
    delete q;

    // 数组
    Tracer* arr = new Tracer[3]{Tracer{1}, Tracer{2}, Tracer{3}};
    assert(arr[2].v == 3);
    delete[] arr;

    // nothrow
    int* n = new (std::nothrow) int{5};
    assert(n && *n == 5);
    delete n;

    std::cout << "  new-expr = allocate + construct; delete = destroy + free\n";
    std::cout << "new_delete_global: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_global", run>;

}  // namespace
