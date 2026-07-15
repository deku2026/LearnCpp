// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocate_at_least_cpp23
// Topic id : part6/c/section06/allocate_at_least_cpp23
//
// Covers: allocator_traits::allocate_at_least (C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

void demo_basics() {
    std::allocator<int> a;
#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
    auto r = std::allocator_traits<std::allocator<int>>::allocate_at_least(a, 4);
    assert(r.count >= 4);
    assert(r.ptr != nullptr);
    std::allocator_traits<std::allocator<int>>::deallocate(a, r.ptr, r.count);
#else
    int* p = a.allocate(4);
    assert(p != nullptr);
    a.deallocate(p, 4);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
    std::allocator<char> a;
    auto r = std::allocator_traits<std::allocator<char>>::allocate_at_least(a, 1);
    assert(r.count >= 1);
    std::allocator_traits<std::allocator<char>>::deallocate(a, r.ptr, r.count);
#else
    assert(true);
#endif
}

void demo_expert() {
    // Containers may over-allocate capacity using allocate_at_least.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocate_at_least_cpp23", run>;

}  // namespace
