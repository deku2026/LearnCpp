// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C6 allocate_at_least C++23)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocate_at_least_cpp23
// Topic id : part6/c/section06/allocate_at_least_cpp23
//
// 要点: allocate_at_least 可返回 ≥ 请求数量的块，减少 vector 再分配次数。
// 参考: https://en.cppreference.com/w/cpp/memory/allocator_traits/allocate_at_least

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C6 allocate_at_least (C++23) ===\n";

    using A = std::allocator<int>;
    using AT = std::allocator_traits<A>;
    A a;

#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202202L
    auto result = AT::allocate_at_least(a, 10);
    assert(result.ptr != nullptr);
    assert(result.count >= 10);
    std::cout << "  requested=10 got count=" << result.count << '\n';
    // 必须用实际 count 去 deallocate
    AT::deallocate(a, result.ptr, result.count);
    std::cout << "  deallocate MUST use returned count, not the request\n";
#else
    int* p = AT::allocate(a, 10);
    assert(p != nullptr);
    AT::deallocate(a, p, 10);
    std::cout << "  allocate_at_least unavailable; classic allocate path\n";
#endif

    std::cout << "  intent: allocator may hand larger slab cheaply\n";
    std::cout << "allocate_at_least_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocate_at_least_cpp23", run>;

}  // namespace
