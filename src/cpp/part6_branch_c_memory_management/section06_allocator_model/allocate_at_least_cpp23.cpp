// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 allocate_at_least)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocate_at_least_cpp23
// Topic id : part6/c/section06/allocate_at_least_cpp23
//
// 要点: allocate_at_least 可返回 count>=n 的更大块，容器可利用额外容量。
// 参考: https://en.cppreference.com/w/cpp/memory/allocator_traits/allocate_at_least

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 allocate_at_least (C++23) ===\n";

    std::allocator<int> alloc;
    using AT = std::allocator_traits<std::allocator<int>>;

#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
    auto result = AT::allocate_at_least(alloc, 10);
    assert(result.ptr != nullptr);
    assert(result.count >= 10);
    std::cout << "  requested 10, got count=" << result.count << '\n';
    for (std::size_t i = 0; i < 10; ++i) result.ptr[i] = static_cast<int>(i);
    assert(result.ptr[9] == 9);
    AT::deallocate(alloc, result.ptr, result.count);
#else
    int* p = AT::allocate(alloc, 10);
    assert(p);
    AT::deallocate(alloc, p, 10);
    std::cout << "  allocate_at_least not available; allocate fallback\n";
#endif

    std::cout << "allocate_at_least_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocate_at_least_cpp23", run>;

}  // namespace
