// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : pmr_memory_resource_cpp17
// Topic id : part3/section06/pmr_memory_resource_cpp17
//
// Covers: std::pmr memory_resource and polymorphic allocator

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory_resource>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    std::pmr::vector<int> v{1, 2, 3};
    assert(v.size() == 3);
    assert(v.get_allocator().resource() != nullptr);
}

void demo_intermediate() {
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::polymorphic_allocator<int> alloc{&pool};
    std::pmr::vector<int> v(alloc);
    v.push_back(10);
    assert(v.front() == 10);
}

void demo_expert() {
    std::pmr::monotonic_buffer_resource mono;
    std::pmr::string s{&mono};
    s = "pmr";
    assert(s == "pmr");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/pmr_memory_resource_cpp17", run>;

}  // namespace
