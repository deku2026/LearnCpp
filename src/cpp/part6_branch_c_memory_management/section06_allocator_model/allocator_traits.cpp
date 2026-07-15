// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_traits
// Topic id : part6/c/section06/allocator_traits
//
// Covers: std::allocator_traits defaults and rebinding

#include "learn/topic_registry.hpp"

#include <memory>

namespace {

void demo_basics() {
    using AT = std::allocator_traits<std::allocator<int>>;
    std::allocator<int> a;
    int* p = AT::allocate(a, 2);
    AT::construct(a, p, 1);
    AT::construct(a, p + 1, 2);
    LEARN_CHECK(p[0] == 1 && p[1] == 2);
    AT::destroy(a, p + 1);
    AT::destroy(a, p);
    AT::deallocate(a, p, 2);
}

void demo_intermediate() {
    using AT = std::allocator_traits<std::allocator<int>>;
    static_assert(std::is_same_v<AT::value_type, int>);
    static_assert(AT::is_always_equal::value);
}

void demo_expert() {
    using AT = std::allocator_traits<std::allocator<int>>;
    using Other = AT::rebind_alloc<double>;
    Other od;
    double* p = std::allocator_traits<Other>::allocate(od, 1);
    std::allocator_traits<Other>::construct(od, p, 1.5);
    LEARN_CHECK(*p == 1.5);
    std::allocator_traits<Other>::destroy(od, p);
    std::allocator_traits<Other>::deallocate(od, p, 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_traits", run>;

}  // namespace
