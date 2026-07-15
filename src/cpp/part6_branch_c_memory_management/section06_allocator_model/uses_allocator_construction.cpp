// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : uses_allocator_construction
// Topic id : part6/c/section06/uses_allocator_construction
//
// Covers: uses-allocator construction protocol

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <scoped_allocator>
#include <vector>

namespace {

void demo_basics() {
    static_assert(std::uses_allocator_v<std::vector<int>, std::allocator<int>>);
}

void demo_intermediate() {
    std::allocator<int> a;
    std::vector<int> v(a);
    v.push_back(1);
    assert(v[0] == 1);
}

void demo_expert() {
#if defined(__cpp_lib_make_obj_using_allocator)
    std::allocator<int> a;
    auto v = std::make_obj_using_allocator<std::vector<int>>(a, 3, 7);
    assert(v.size() == 3);
    assert(v[0] == 7);
#else
    std::vector<int> v(3, 7);
    assert(v.size() == 3);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/uses_allocator_construction", run>;

}  // namespace
