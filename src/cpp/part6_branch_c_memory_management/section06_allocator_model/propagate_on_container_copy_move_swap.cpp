// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 传播 traits)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : propagate_on_container_copy_move_swap
// Topic id : part6/c/section06/propagate_on_container_copy_move_swap
//
// 要点: POCCA/POCMA/POCS 控制拷贝赋值/移动赋值/swap 时分配器是否传播。
// 参考: AllocatorAwareContainer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <type_traits>

namespace {

template <typename T>
struct StickyAlloc {
    using value_type = T;
    int id = 0;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    StickyAlloc() = default;
    explicit StickyAlloc(int i) : id(i) {}
    template <typename U>
    StickyAlloc(const StickyAlloc<U>& o) noexcept : id(o.id) {}

    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
    friend bool operator==(const StickyAlloc& a, const StickyAlloc& b) noexcept { return a.id == b.id; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 POCCA / POCMA / POCS ===\n";

    using AT = std::allocator_traits<StickyAlloc<int>>;
    static_assert(AT::propagate_on_container_copy_assignment::value);
    static_assert(AT::propagate_on_container_move_assignment::value);
    static_assert(AT::propagate_on_container_swap::value);

    // 默认 std::allocator：POCCA/POCS 不传播；POCMA 为 true（allocator 自带）
    using DA = std::allocator_traits<std::allocator<int>>;
    static_assert(!DA::propagate_on_container_copy_assignment::value);
    static_assert(DA::propagate_on_container_move_assignment::value);
    static_assert(!DA::propagate_on_container_swap::value);

    StickyAlloc<int> a{1}, b{2};
    assert(a.id == 1 && b.id == 2);
    assert(!(a == b));

    std::cout << "  default false: keep own alloc; swap unequal allocators = UB\n";
    std::cout << "  true: allocator travels with assignment/swap\n";
    std::cout << "propagate_on_container_copy_move_swap: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/propagate_on_container_copy_move_swap", run>;

}  // namespace
