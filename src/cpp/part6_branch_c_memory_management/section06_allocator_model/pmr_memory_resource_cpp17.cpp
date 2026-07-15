// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pmr_memory_resource_cpp17
// Topic id : part6/c/section06/pmr_memory_resource_cpp17
//
// Covers: std::pmr::memory_resource and polymorphic_allocator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory_resource>
#include <vector>

namespace {

void demo_basics() {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<int> v{&mr};
    v.push_back(1);
    assert(v[0] == 1);
}

void demo_intermediate() {
    alignas(int) char buf[1024];
    std::pmr::monotonic_buffer_resource mr(buf, sizeof(buf));
    std::pmr::vector<int> v{&mr};
    for (int i = 0; i < 10; ++i) v.push_back(i);
    assert(v.size() == 10);
}

void demo_expert() {
    auto* up = std::pmr::new_delete_resource();
    void* p = up->allocate(16, alignof(std::max_align_t));
    assert(p != nullptr);
    up->deallocate(p, 16, alignof(std::max_align_t));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pmr_memory_resource_cpp17", run>;

}  // namespace
