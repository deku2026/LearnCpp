// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : container_alloc_aware_behavior
// Topic id : part6/c/section06/container_alloc_aware_behavior
//
// Covers: allocator-aware containers propagate/select on copy/move

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory_resource>
#include <vector>

namespace {

void demo_basics() {
    char buf[1024];
    std::pmr::monotonic_buffer_resource mr(buf, sizeof(buf));
    std::pmr::vector<int> a{&mr};
    a.push_back(1);
    std::pmr::vector<int> b = a;
    assert(b[0] == 1);
}

void demo_intermediate() {
    std::pmr::vector<int> a{1, 2, 3};
    std::pmr::vector<int> b = std::move(a);
    assert(b.size() == 3);
}

void demo_expert() {
    char buf1[512], buf2[512];
    std::pmr::monotonic_buffer_resource mr1(buf1, sizeof(buf1));
    std::pmr::monotonic_buffer_resource mr2(buf2, sizeof(buf2));
    std::pmr::vector<int> a{&mr1};
    a.push_back(9);
    std::pmr::vector<int> b{&mr2};
    b = a;  // may allocate in mr2
    assert(b[0] == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/container_alloc_aware_behavior", run>;

}  // namespace
