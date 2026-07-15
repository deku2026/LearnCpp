// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C8 pool_resource)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pool_resource
// Topic id : part6/c/section06/pool_resource
//
// 要点: unsynchronized_pool_resource 分桶池（单线程）；synchronized 带锁。
// 参考: pmr pool resources

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <list>
#include <memory_resource>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C8 pmr pool_resource ===\n";

    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::list<int> L{&pool};
    for (int i = 0; i < 50; ++i) L.push_back(i);
    assert(L.size() == 50);
    assert(L.front() == 0);
    assert(L.back() == 49);

    // synchronized 版本：多线程安全（本例单线程使用）
    std::pmr::synchronized_pool_resource sync_pool;
    std::pmr::vector<int> v{&sync_pool};
    v.resize(20, 7);
    assert(v.size() == 20 && v[0] == 7);

    std::cout << "  pools recycle blocks by size class; good for node containers\n";
    std::cout << "pool_resource: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pool_resource", run>;

}  // namespace
