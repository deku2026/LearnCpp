// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : std_allocator
// Topic id : part3/section06/std_allocator
//
// Covers: std::allocator allocate/construct/deallocate

#include "learn/topic_registry.hpp"

#include <memory>
#include <vector>

namespace {

void demo_basics() {
    std::allocator<int> alloc;
    int* p = alloc.allocate(1);
    std::allocator_traits<std::allocator<int>>::construct(alloc, p, 42);
    LEARN_CHECK(*p == 42);
    std::allocator_traits<std::allocator<int>>::destroy(alloc, p);
    alloc.deallocate(p, 1);
}

void demo_intermediate() {
    std::vector<int, std::allocator<int>> v{1, 2, 3};
    LEARN_CHECK(v.size() == 3);
}

void demo_expert() {
    using Traits = std::allocator_traits<std::allocator<int>>;
    static_assert(std::is_same_v<Traits::value_type, int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/std_allocator", run>;

}  // namespace
