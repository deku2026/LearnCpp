// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C2 全局 new/delete)
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_global
// Topic id : part6/c/section01/new_delete_global
//
// 要点: new 表达式 = operator new 分配 + 构造；delete = 析构 + operator delete。
//       不替换全局 new（避免污染进程）；用 ::operator new 直接演示。
// 参考: https://en.cppreference.com/w/cpp/memory/new/operator_new

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>

namespace {

struct Tracer {
    int v;
    explicit Tracer(int x) : v(x) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C2 global operator new / delete (via ::) ===\n";

    // --- 入门: 分配原始内存 + placement 构造 ---
    void* raw = ::operator new(sizeof(Tracer));
    assert(raw != nullptr);
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
    assert(arr[0].v == 1 && arr[2].v == 3);
    delete[] arr;

    // nothrow
    int* n = new (std::nothrow) int{5};
    assert(n && *n == 5);
    delete n;

    // 进阶: 对齐版（C++17）
    void* aligned = ::operator new(64, std::align_val_t{32});
    assert(aligned != nullptr);
    ::operator delete(aligned, std::align_val_t{32});

    // 专家: 全局替换 operator new 影响全程序（调试器/池/统计）；
    // 还应配套 new[]/delete[]、nothrow、aligned 全套
    std::cout << "  new-expr = allocate + construct; delete = destroy + free\n";
    std::cout << "  operator new alone does NOT run constructors\n";
    std::cout << "new_delete_global: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_global", run>;

}  // namespace
