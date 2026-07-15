// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : allocate_at_least_cpp23
// Topic id : part3/section06/allocate_at_least_cpp23
//
// Covers: allocator_traits::allocate_at_least C++23

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    std::allocator<int> a;
    int* p = a.allocate(4);
    assert(p != nullptr);
    a.deallocate(p, 4);
}

void demo_intermediate() {
#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202106L
    std::allocator<int> a;
    auto r = std::allocator_traits<std::allocator<int>>::allocate_at_least(a, 8);
    assert(r.ptr != nullptr);
    assert(r.count >= 8);
    a.deallocate(r.ptr, r.count);
#else
    assert(true);
#endif
}

void demo_expert() {
    // vector capacity growth is related idea: may allocate more than size
    std::vector<int> v;
    v.reserve(8);
    assert(v.capacity() >= 8);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/allocate_at_least_cpp23", run>;

}  // namespace
