// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C7/C8 线性 arena)
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : linear_arena_allocator
// Topic id : part6/c/section03/linear_arena_allocator
//
// 要点: bump-pointer 线性分配；只增长，整块重置；适合帧/请求临时对象。
// 参考: monotonic 分配思想

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <vector>

namespace {

class LinearArena {
    std::byte* base_ = nullptr;
    std::size_t size_ = 0;
    std::size_t offset_ = 0;

public:
    LinearArena(std::byte* b, std::size_t n) : base_(b), size_(n) {}

    void* allocate(std::size_t bytes, std::size_t align) {
        auto cur = reinterpret_cast<std::uintptr_t>(base_ + offset_);
        auto aligned = (cur + (align - 1)) & ~(align - 1);
        auto next = aligned + bytes;
        auto end = reinterpret_cast<std::uintptr_t>(base_ + size_);
        if (next > end) throw std::bad_alloc{};
        offset_ = static_cast<std::size_t>(next - reinterpret_cast<std::uintptr_t>(base_));
        return reinterpret_cast<void*>(aligned);
    }
    void reset() { offset_ = 0; }
    std::size_t used() const { return offset_; }
};

template <typename T>
struct ArenaAllocator {
    using value_type = T;
    LinearArena* arena = nullptr;

    ArenaAllocator() = default;
    explicit ArenaAllocator(LinearArena& a) : arena(&a) {}
    template <typename U>
    ArenaAllocator(const ArenaAllocator<U>& o) noexcept : arena(o.arena) {}

    T* allocate(std::size_t n) { return static_cast<T*>(arena->allocate(n * sizeof(T), alignof(T))); }
    void deallocate(T*, std::size_t) noexcept {
        // 线性 arena 不单独释放
    }
    friend bool operator==(const ArenaAllocator& a, const ArenaAllocator& b) noexcept { return a.arena == b.arena; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C linear arena allocator ===\n";

    alignas(std::max_align_t) std::byte buffer[4096];
    LinearArena arena{buffer, sizeof(buffer)};
    ArenaAllocator<int> alloc{arena};

    std::vector<int, ArenaAllocator<int>> v{alloc};
    for (int i = 0; i < 100; ++i) v.push_back(i);
    assert(v.size() == 100);
    assert(v[50] == 50);
    assert(arena.used() > 0);

    arena.reset();
    assert(arena.used() == 0);

    std::cout << "  bump alloc; reset frees all at once\n";
    std::cout << "  [layer] traits/pmr/pools: see branch C doc C6-C8 acceptance\n";
    std::cout << "linear_arena_allocator: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/linear_arena_allocator", run>;

}  // namespace
