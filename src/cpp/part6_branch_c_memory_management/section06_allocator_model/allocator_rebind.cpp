// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 rebind)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_rebind
// Topic id : part6/c/section06/allocator_rebind
//
// 要点: list 等需分配 Node 而非 T；traits::rebind_alloc 自动重绑定模板分配器。
// 参考: allocator_traits::rebind_alloc

#include "learn/topic_registry.hpp"

#include <iostream>
#include <list>
#include <memory>
#include <new>
#include <type_traits>

namespace {

// 跨 rebind 共享计数（CountingAlloc<Node> 与 CountingAlloc<int> 不是同一静态）
inline std::size_t g_last_alloc_bytes = 0;

template <typename T>
struct CountingAlloc {
    using value_type = T;

    CountingAlloc() = default;
    template <typename U>
    CountingAlloc(const CountingAlloc<U>&) noexcept {}

    T* allocate(std::size_t n) {
        g_last_alloc_bytes = n * sizeof(T);
        return static_cast<T*>(::operator new(g_last_alloc_bytes));
    }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
    friend bool operator==(const CountingAlloc&, const CountingAlloc&) noexcept { return true; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 allocator rebind ===\n";

    using AInt = CountingAlloc<int>;
    using AChar = std::allocator_traits<AInt>::rebind_alloc<char>;
    static_assert(std::is_same_v<AChar, CountingAlloc<char>>);

    g_last_alloc_bytes = 0;
    std::list<int, CountingAlloc<int>> L;
    L.push_back(42);
    // rebind 后分配的是链表节点，字节数 > sizeof(int)
    assert(g_last_alloc_bytes > sizeof(int));
    assert(L.front() == 42);
    std::cout << "  node alloc bytes=" << g_last_alloc_bytes << " ( > sizeof(int) )\n";

    std::cout << "  [layer] traits/pmr/pools: see branch C doc C6-C8 acceptance\n";
    std::cout << "allocator_rebind: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_rebind", run>;

}  // namespace
