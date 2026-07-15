// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_rebind
// Topic id : part6/c/section06/allocator_rebind
//
// Covers: allocator rebind for node-based containers

#include "learn/topic_registry.hpp"

#include <list>
#include <memory>

namespace {

void demo_basics() {
    using A = std::allocator<int>;
    using AT = std::allocator_traits<A>;
    using NodeAlloc = AT::rebind_alloc<double>;
    static_assert(std::is_same_v<NodeAlloc::value_type, double>);
}

void demo_intermediate() {
    std::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    LEARN_CHECK(lst.front() == 1 && lst.back() == 2);
}

void demo_expert() {
    // Containers rebind the user allocator to their internal node type.
    std::list<int, std::allocator<int>> lst{3, 4};
    LEARN_CHECK(lst.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_rebind", run>;

}  // namespace
