// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_requirements
// Topic id : part6/c/section06/allocator_requirements
//
// Covers: Allocator named requirements sketch

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace {

void demo_basics() {
    std::allocator<int> a;
    int* p = a.allocate(1);
    std::allocator_traits<std::allocator<int>>::construct(a, p, 5);
    assert(*p == 5);
    std::allocator_traits<std::allocator<int>>::destroy(a, p);
    a.deallocate(p, 1);
}

void demo_intermediate() {
    std::vector<int> v(std::allocator<int>{});
    v.push_back(1);
    assert(v.front() == 1);
}

void demo_expert() {
    using A = std::allocator<int>;
    static_assert(std::is_same_v<A::value_type, int>);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_requirements", run>;

}  // namespace
