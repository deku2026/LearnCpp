// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C 固定块内存池)
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : pool_allocator
// Topic id : part6/c/section03/pool_allocator
//
// 要点: 固定大小块 freelist 池——O(1) 分配/释放同类小对象。
// 参考: pool allocator pattern

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <new>
#include <vector>

namespace {

template <std::size_t BlockSize, std::size_t BlockCount>
class FixedPool {
    alignas(std::max_align_t) std::byte storage_[BlockSize * BlockCount]{};
    void* free_ = nullptr;

public:
    FixedPool() {
        for (std::size_t i = 0; i < BlockCount; ++i) {
            void* blk = storage_ + i * BlockSize;
            *static_cast<void**>(blk) = free_;
            free_ = blk;
        }
    }
    void* allocate() {
        if (!free_) throw std::bad_alloc{};
        void* p = free_;
        free_ = *static_cast<void**>(free_);
        return p;
    }
    void deallocate(void* p) noexcept {
        *static_cast<void**>(p) = free_;
        free_ = p;
    }
};

struct Node {
    int value;
    Node* next;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C fixed-block pool allocator ===\n";

    FixedPool<sizeof(Node), 32> pool;
    std::vector<Node*> nodes;
    for (int i = 0; i < 10; ++i) {
        auto* n = static_cast<Node*>(pool.allocate());
        n->value = i;
        n->next = nullptr;
        nodes.push_back(n);
    }
    assert(nodes[5]->value == 5);

    for (auto* n : nodes) pool.deallocate(n);

    // 复用
    auto* again = static_cast<Node*>(pool.allocate());
    again->value = 99;
    assert(again->value == 99);
    pool.deallocate(again);

    std::cout << "  freelist O(1) for fixed-size objects\n";
    std::cout << "pool_allocator: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/pool_allocator", run>;

}  // namespace
