// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : memory_headers
// Topic id : part3/section08/memory_headers
//
// Covers: headers map: memory memory_resource new scoped_allocator

#include "learn/topic_registry.hpp"

#include <memory>
#include <memory_resource>
#include <new>
#include <vector>

namespace {

void demo_basics() {
    auto p = std::make_unique<int>(1);
    LEARN_CHECK(*p == 1);
}

void demo_intermediate() {
    std::pmr::vector<int> v{1, 2};
    LEARN_CHECK(v.size() == 2);
}

void demo_expert() {
    void* raw = ::operator new(16);
    ::operator delete(raw);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/memory_headers", run>;

}  // namespace
