// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C2 类级 new/delete)
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_class_level
// Topic id : part6/c/section01/new_delete_class_level
//
// 要点: 类级 operator new/delete 只影响该类；常用于池/统计。隐式 static。
// 参考: [class.free]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>

namespace {

struct Pooled {
    int x = 0;
    static int alloc_count;
    static int free_count;

    static void* operator new(std::size_t size) {
        ++alloc_count;
        return ::operator new(size);
    }
    static void operator delete(void* p) noexcept {
        ++free_count;
        ::operator delete(p);
    }
    // 进阶: 数组形式可单独重载
    static void* operator new[](std::size_t size) {
        ++alloc_count;
        return ::operator new[](size);
    }
    static void operator delete[](void* p) noexcept {
        ++free_count;
        ::operator delete[](p);
    }
};
int Pooled::alloc_count = 0;
int Pooled::free_count = 0;

struct Ordinary {
    int y = 0;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C2 class-level operator new/delete ===\n";

    Pooled::alloc_count = 0;
    Pooled::free_count = 0;

    auto* p = new Pooled{};
    p->x = 9;
    assert(p->x == 9);
    assert(Pooled::alloc_count == 1);
    delete p;
    assert(Pooled::free_count == 1);

    auto* arr = new Pooled[2];
    assert(Pooled::alloc_count == 2);
    delete[] arr;
    assert(Pooled::free_count == 2);

    // 其它类型不受影响
    auto* o = new Ordinary{};
    assert(o->y == 0);
    delete o;
    assert(Pooled::alloc_count == 2);

    // 栈对象不走 operator new
    Pooled stack{};
    assert(Pooled::alloc_count == 2);
    (void)stack;

    std::cout << "  class new is static; only for that class's new-expr\n";
    std::cout << "  use cases: type-specific pools, leak stats, special memory\n";
    std::cout << "new_delete_class_level: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_class_level", run>;

}  // namespace
