// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : monotonic_buffer_resource_cpp17
// Topic id : part3/section06/monotonic_buffer_resource_cpp17
//
// Covers: pmr::monotonic_buffer_resource arena allocation

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <memory_resource>
#include <vector>

namespace {

void demo_basics() {
    std::array<std::byte, 1024> buffer{};
    std::pmr::monotonic_buffer_resource mono{buffer.data(), buffer.size()};
    std::pmr::vector<int> v{&mono};
    v.push_back(1);
    v.push_back(2);
    assert(v.size() == 2);
}

void demo_intermediate() {
    std::pmr::monotonic_buffer_resource mono;
    std::pmr::vector<int> a{&mono};
    std::pmr::vector<int> b{&mono};
    a.resize(10, 1);
    b.resize(10, 2);
    assert(a[0] == 1 && b[0] == 2);
}

void demo_expert() {
    // release() frees upstream; objects must not outlive resource
    std::pmr::monotonic_buffer_resource mono;
    {
        std::pmr::vector<int> v{&mono};
        v.push_back(3);
        assert(v.back() == 3);
    }
    mono.release();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/monotonic_buffer_resource_cpp17", run>;

}  // namespace
