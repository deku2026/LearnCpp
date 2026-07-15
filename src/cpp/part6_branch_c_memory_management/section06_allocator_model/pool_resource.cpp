// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C8 pool_resource)
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : pool_resource
// Topic id : part6/c/section06/pool_resource
//
// 要点: unsynchronized_pool_resource 分桶池（单线程）；synchronized 带锁。
// 适合 list/map 等节点容器的同尺寸反复分配。
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

    // --- 入门: 无锁池 + 节点容器 ---
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::list<int> L{&pool};
    for (int i = 0; i < 50; ++i) L.push_back(i);
    assert(L.size() == 50);
    assert(L.front() == 0 && L.back() == 49);

    // 删除后再插入：块回池，避免反复问上游
    for (int i = 0; i < 25; ++i) L.pop_front();
    for (int i = 0; i < 25; ++i) L.push_back(100 + i);
    assert(L.size() == 50);

    // --- 进阶: synchronized 多线程安全（本例单线程使用）---
    std::pmr::synchronized_pool_resource sync_pool;
    std::pmr::vector<int> v{&sync_pool};
    v.resize(20, 7);
    assert(v.size() == 20 && v[0] == 7);

    // 专家: vs monotonic — pool 可按 size class 回收；monotonic 只涨不还
    // unsynchronized 更快，禁止跨线程共享同一 resource
    std::cout << "  pools recycle blocks by size class; good for node containers\n";
    std::cout << "  unsynchronized: single-thread only; synchronized: mutex inside\n";
    std::cout << "pool_resource: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/pool_resource", run>;

}  // namespace
