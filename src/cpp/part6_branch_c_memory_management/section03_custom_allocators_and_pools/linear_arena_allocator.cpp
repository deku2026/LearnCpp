// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : linear_arena_allocator
// Topic id : part6/c/section03/linear_arena_allocator
//
// Covers: bump-pointer / linear arena allocation pattern

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>

namespace {

class Arena {
    unsigned char* buf_;
    std::size_t cap_;
    std::size_t off_ = 0;

public:
    Arena(unsigned char* b, std::size_t c) : buf_(b), cap_(c) {}
    void* allocate(std::size_t n, std::size_t align) {
        std::uintptr_t base = reinterpret_cast<std::uintptr_t>(buf_ + off_);
        std::uintptr_t aligned = (base + align - 1) & ~(align - 1);
        std::size_t padding = static_cast<std::size_t>(aligned - base);
        if (off_ + padding + n > cap_) return nullptr;
        off_ += padding + n;
        return reinterpret_cast<void*>(aligned);
    }
    void reset() { off_ = 0; }
    std::size_t used() const { return off_; }
};

void demo_basics() {
    alignas(16) unsigned char storage[256];
    Arena a(storage, sizeof(storage));
    void* p = a.allocate(16, 8);
    LEARN_CHECK(p != nullptr);
}

void demo_intermediate() {
    alignas(16) unsigned char storage[128];
    Arena a(storage, sizeof(storage));
    int* i = static_cast<int*>(a.allocate(sizeof(int), alignof(int)));
    double* d = static_cast<double*>(a.allocate(sizeof(double), alignof(double)));
    LEARN_CHECK(i && d);
    *i = 3;
    *d = 1.5;
    LEARN_CHECK(*i == 3 && *d == 1.5);
}

void demo_expert() {
    alignas(16) unsigned char storage[64];
    Arena a(storage, sizeof(storage));
    (void)a.allocate(32, 8);
    a.reset();
    LEARN_CHECK(a.used() == 0);
    LEARN_CHECK(a.allocate(64, 8) != nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/linear_arena_allocator", run>;

}  // namespace
