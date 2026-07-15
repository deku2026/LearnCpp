// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : uses_allocator_construction
// Topic id : part3/section06/uses_allocator_construction
//
// Covers: uses-allocator construction protocol

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <memory_resource>
#include <scoped_allocator>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    std::allocator<int> a;
    std::vector<int> v(a);
    v.push_back(1);
    assert(v.size() == 1);
}

void demo_intermediate() {
    std::pmr::vector<std::pmr::string> v;
    v.emplace_back("hi");
    assert(v.front() == "hi");
}

void demo_expert() {
    std::pmr::monotonic_buffer_resource mono;
    std::pmr::vector<std::pmr::string> v{&mono};
    v.emplace_back("arena");
    assert(v.front() == "arena");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/uses_allocator_construction", run>;

}  // namespace
