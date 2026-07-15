// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6/C7 allocator-aware 容器)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : container_alloc_aware_behavior
// Topic id : part6/c/section06/container_alloc_aware_behavior
//
// 要点: allocator-aware 容器在构造/拷贝/移动时携带分配器；get_allocator 可查。
// 参考: AllocatorAwareContainer

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace {

template <typename T>
struct IdAlloc {
    using value_type = T;
    int id = 0;
    using is_always_equal = std::false_type;
    IdAlloc() = default;
    explicit IdAlloc(int i) : id(i) {}
    template <typename U>
    IdAlloc(const IdAlloc<U>& o) noexcept : id(o.id) {}
    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
    friend bool operator==(const IdAlloc& a, const IdAlloc& b) noexcept { return a.id == b.id; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 container allocator-aware behavior ===\n";

    IdAlloc<int> a1{1};
    IdAlloc<int> a2{2};
    std::vector<int, IdAlloc<int>> v1{a1};
    v1.push_back(10);
    assert(v1.get_allocator().id == 1);

    // 用 a2 构造另一容器
    std::vector<int, IdAlloc<int>> v2{a2};
    v2.push_back(20);
    assert(v2.get_allocator().id == 2);

    // 拷贝构造：分配器选择规则由 allocator_traits select_on_container_copy_construction 决定
    auto v3 = v1;
    assert(v3.size() == 1 && v3[0] == 10);

    std::cout << "  get_allocator() exposes the container's allocator instance\n";
    std::cout << "  unequal allocators: move/swap rules depend on POC* traits\n";
    std::cout << "container_alloc_aware_behavior: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/container_alloc_aware_behavior", run>;

}  // namespace
