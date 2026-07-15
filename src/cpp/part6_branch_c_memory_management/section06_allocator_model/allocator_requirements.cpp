// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 Allocator 最小要求)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_requirements
// Topic id : part6/c/section06/allocator_requirements
//
// 要点: 最小集 value_type + allocate + deallocate (+ 相等)；其余可选。
// 参考: https://en.cppreference.com/w/cpp/named_req/Allocator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>

namespace {

template <typename T>
struct MinimalAlloc {
    using value_type = T;
    MinimalAlloc() = default;
    template <typename U>
    MinimalAlloc(const MinimalAlloc<U>&) noexcept {}
    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
    friend bool operator==(const MinimalAlloc&, const MinimalAlloc&) noexcept { return true; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 allocator requirements (minimal) ===\n";

    MinimalAlloc<int> a;
    int* p = std::allocator_traits<MinimalAlloc<int>>::allocate(a, 4);
    for (int i = 0; i < 4; ++i) {
        std::allocator_traits<MinimalAlloc<int>>::construct(a, p + i, i + 1);
    }
    assert(p[0] == 1 && p[3] == 4);
    for (int i = 0; i < 4; ++i) {
        std::allocator_traits<MinimalAlloc<int>>::destroy(a, p + i);
    }
    std::allocator_traits<MinimalAlloc<int>>::deallocate(a, p, 4);

    std::cout << "  optional: construct/destroy/rebind/propagate traits\n";
    std::cout << "  [layer] traits/pmr/pools: see branch C doc C6-C8 acceptance\n";
    std::cout << "allocator_requirements: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_requirements", run>;

}  // namespace
