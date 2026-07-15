// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : custom_allocator_for_vector
// Topic id : part6/c/section03/custom_allocator_for_vector
//
// Covers: minimal allocator usable with std::vector

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace {

template <class T>
struct CountingAllocator {
    using value_type = T;
    static inline int allocs = 0;
    CountingAllocator() = default;
    template <class U>
    CountingAllocator(const CountingAllocator<U>&) noexcept {}
    T* allocate(std::size_t n) {
        ++allocs;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
};
template <class T, class U>
bool operator==(const CountingAllocator<T>&, const CountingAllocator<U>&) {
    return true;
}
template <class T, class U>
bool operator!=(const CountingAllocator<T>&, const CountingAllocator<U>&) {
    return false;
}

void demo_basics() {
    CountingAllocator<int>::allocs = 0;
    std::vector<int, CountingAllocator<int>> v;
    v.push_back(1);
    assert(v[0] == 1);
    assert(CountingAllocator<int>::allocs >= 1);
}

void demo_intermediate() {
    std::vector<int, CountingAllocator<int>> v{1, 2, 3};
    assert(v.size() == 3);
}

void demo_expert() {
    CountingAllocator<int>::allocs = 0;
    {
        std::vector<int, CountingAllocator<int>> v;
        v.reserve(32);
        assert(CountingAllocator<int>::allocs == 1);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/custom_allocator_for_vector", run>;

}  // namespace
