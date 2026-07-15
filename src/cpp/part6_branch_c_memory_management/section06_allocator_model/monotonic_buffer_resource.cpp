// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : monotonic_buffer_resource
// Topic id : part6/c/section06/monotonic_buffer_resource
//
// Covers: pmr::monotonic_buffer_resource bump allocation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory_resource>
#include <string>

namespace {

void demo_basics() {
    char buf[512];
    std::pmr::monotonic_buffer_resource mr(buf, sizeof(buf));
    std::pmr::string s{&mr};
    s = "hello";
    assert(s == "hello");
}

void demo_intermediate() {
    char buf[256];
    std::pmr::monotonic_buffer_resource mr(buf, sizeof(buf));
    void* a = mr.allocate(32);
    void* b = mr.allocate(32);
    assert(a != b);
    // individual deallocate is no-op for monotonic; release on destruction/reset
    (void)a;
    (void)b;
}

void demo_expert() {
    std::pmr::monotonic_buffer_resource mr;
    std::pmr::vector<int> v{&mr};
    v.resize(100, 1);
    assert(v[99] == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/monotonic_buffer_resource", run>;

}  // namespace
