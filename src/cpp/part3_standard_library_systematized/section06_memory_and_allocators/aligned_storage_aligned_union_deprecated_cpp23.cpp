// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : aligned_storage_aligned_union_deprecated_cpp23
// Topic id : part3/section06/aligned_storage_aligned_union_deprecated_cpp23
//
// Covers: aligned_storage/aligned_union deprecated; prefer alignas + array

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace {

void demo_basics() {
    alignas(double) unsigned char buf[sizeof(double)];
    LEARN_CHECK(reinterpret_cast<std::uintptr_t>(buf) % alignof(double) == 0 || true);
    (void)buf;
}

void demo_intermediate() {
    // modern replacement for aligned_storage
    struct Storage {
        alignas(std::max_align_t) unsigned char bytes[64];
    };
    Storage s{};
    LEARN_CHECK(sizeof(s.bytes) == 64);
}

void demo_expert() {
#if defined(__cpp_lib_byte)
    // aligned_storage_t is deprecated in C++23 — avoid in new code
#endif
    static_assert(alignof(std::max_align_t) >= alignof(void*));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/aligned_storage_aligned_union_deprecated_cpp23", run>;

}  // namespace
