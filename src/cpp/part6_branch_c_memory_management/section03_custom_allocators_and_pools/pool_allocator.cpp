// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : pool_allocator
// Topic id : part6/c/section03/pool_allocator
//
// Covers: fixed-size free-list pool allocator sketch

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <vector>

namespace {

class IntPool {
    // Slot must hold either free-list link or an int (pointer may be > sizeof(int)).
    union Slot {
        Slot* next;
        int value;
    };
    std::vector<Slot> storage_;
    Slot* free_ = nullptr;

public:
    explicit IntPool(std::size_t count) : storage_(count) {
        for (std::size_t i = 0; i < count; ++i) {
            storage_[i].next = free_;
            free_ = &storage_[i];
        }
    }
    int* allocate() {
        if (!free_) return nullptr;
        Slot* s = free_;
        free_ = free_->next;
        return &s->value;
    }
    void deallocate(int* p) {
        auto* s = reinterpret_cast<Slot*>(p);
        s->next = free_;
        free_ = s;
    }
};

void demo_basics() {
    IntPool pool(4);
    int* p = pool.allocate();
    assert(p);
    *p = 9;
    assert(*p == 9);
    pool.deallocate(p);
}

void demo_intermediate() {
    IntPool pool(2);
    int* a = pool.allocate();
    int* b = pool.allocate();
    assert(a && b && a != b);
    assert(pool.allocate() == nullptr);
    pool.deallocate(a);
    assert(pool.allocate() != nullptr);
    pool.deallocate(b);
}

void demo_expert() {
    IntPool pool(8);
    int* arr[8];
    for (int i = 0; i < 8; ++i) {
        arr[i] = pool.allocate();
        assert(arr[i]);
        *arr[i] = i;
    }
    for (int i = 0; i < 8; ++i) assert(*arr[i] == i);
    for (int i = 0; i < 8; ++i) pool.deallocate(arr[i]);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/pool_allocator", run>;

}  // namespace
