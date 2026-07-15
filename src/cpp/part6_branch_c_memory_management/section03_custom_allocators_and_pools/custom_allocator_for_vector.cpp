// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C7 最小自定义分配器)
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : custom_allocator_for_vector
// Topic id : part6/c/section03/custom_allocator_for_vector
//
// 要点: 只需 value_type/allocate/deallocate；traits 补齐其余。
// 验收: 写一个最小 allocator-aware 自定义分配器挂 vector。
// 参考: [allocator.requirements]

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <iostream>
#include <new>
#include <vector>

namespace {

template <typename T>
struct LoggingAllocator {
    using value_type = T;
    static inline int allocations = 0;
    static inline std::size_t bytes = 0;

    LoggingAllocator() = default;
    template <typename U>
    LoggingAllocator(const LoggingAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        ++allocations;
        bytes += n * sizeof(T);
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }

    friend bool operator==(const LoggingAllocator&, const LoggingAllocator&) noexcept { return true; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C7 custom allocator for vector ===\n";

    LoggingAllocator<int>::allocations = 0;
    LoggingAllocator<int>::bytes = 0;
    {
        std::vector<int, LoggingAllocator<int>> v;
        v.reserve(8);
        for (int i = 0; i < 8; ++i) v.push_back(i);
        assert(v.size() == 8);
        assert(v[7] == 7);
        assert(LoggingAllocator<int>::allocations >= 1);
        assert(LoggingAllocator<int>::bytes >= 8 * sizeof(int));
    }

    std::cout << "  allocations=" << LoggingAllocator<int>::allocations << " bytes>=" << LoggingAllocator<int>::bytes
              << '\n';
    std::cout << "  minimal allocator: value_type + allocate + deallocate + ==\n";
    std::cout << "custom_allocator_for_vector: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/custom_allocator_for_vector", run>;

}  // namespace
