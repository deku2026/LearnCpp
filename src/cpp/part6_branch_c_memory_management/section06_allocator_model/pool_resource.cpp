// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pool_resource
// Topic id : part6/c/section06/pool_resource
//
// Covers: pmr unsynchronized/synchronized pool resource

#include "learn/topic_registry.hpp"

#include <memory_resource>
#include <vector>

namespace {

void demo_basics() {
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::vector<int> v{&pool};
    v.push_back(5);
    LEARN_CHECK(v.back() == 5);
}

void demo_intermediate() {
    std::pmr::synchronized_pool_resource pool;
    std::pmr::vector<int> v{&pool};
    for (int i = 0; i < 50; ++i) v.push_back(i);
    LEARN_CHECK(v.size() == 50);
}

void demo_expert() {
    std::pmr::unsynchronized_pool_resource pool;
    void* p = pool.allocate(64);
    LEARN_CHECK(p != nullptr);
    pool.deallocate(p, 64);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pool_resource", run>;

}  // namespace
