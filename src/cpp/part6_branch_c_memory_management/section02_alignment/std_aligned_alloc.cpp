// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : std_aligned_alloc
// Topic id : part6/c/section02/std_aligned_alloc
//
// Covers: aligned allocation APIs (std::aligned_alloc when available)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <memory>

namespace {

void demo_basics() {
#if defined(__cpp_aligned_new)
    void* p = ::operator new(64, std::align_val_t{64});
    assert(p != nullptr);
    assert(reinterpret_cast<std::uintptr_t>(p) % 64u == 0);
    ::operator delete(p, std::align_val_t{64});
#else
    alignas(64) char buf[64];
    assert(reinterpret_cast<std::uintptr_t>(buf) % 64u == 0);
#endif
}

void demo_intermediate() {
    auto deleter = [](void* p) { ::operator delete(p, std::align_val_t{32}); };
    std::unique_ptr<void, decltype(deleter)> p(::operator new(32, std::align_val_t{32}), deleter);
    assert(reinterpret_cast<std::uintptr_t>(p.get()) % 32u == 0);
}

void demo_expert() {
    // Prefer C++ aligned new over platform-specific aligned_alloc for portability.
    void* p = ::operator new(128, std::align_val_t{64});
    assert(p != nullptr);
    ::operator delete(p, std::align_val_t{64});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/std_aligned_alloc", run>;

}  // namespace
