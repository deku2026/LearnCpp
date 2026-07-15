// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : pool_resource_cpp17
// Topic id : part3/section06/pool_resource_cpp17
//
// Covers: pmr synchronized/unsynchronized pool resources

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory_resource>
#include <vector>

namespace {

void demo_basics() {
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::vector<int> v{&pool};
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }
    assert(v.size() == 100);
}

void demo_intermediate() {
    std::pmr::synchronized_pool_resource pool;
    std::pmr::vector<int> v{&pool};
    v.push_back(7);
    assert(v.front() == 7);
}

void demo_expert() {
    std::pmr::pool_options opts{};
    opts.max_blocks_per_chunk = 16;
    opts.largest_required_pool_block = 256;
    std::pmr::unsynchronized_pool_resource pool{opts};
    std::pmr::vector<char> v{&pool};
    v.resize(128, 'x');
    assert(v.size() == 128);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/pool_resource_cpp17", run>;

}  // namespace
