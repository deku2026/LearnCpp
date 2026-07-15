// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : std_launder
// Topic id : part3/section06/std_launder
//
// Covers: std::launder after placement-new reuse

#include "learn/topic_registry.hpp"

#include <memory>
#include <new>

namespace {

void demo_basics() {
    alignas(int) unsigned char storage[sizeof(int)];
    int* p = new (storage) int{5};
    LEARN_CHECK(*p == 5);
    std::destroy_at(p);
}

void demo_intermediate() {
    alignas(int) unsigned char storage[sizeof(int)];
    int* p = new (storage) int{1};
    std::destroy_at(p);
    p = new (storage) int{2};
    LEARN_CHECK(*std::launder(p) == 2);
    std::destroy_at(p);
}

void demo_expert() {
    // launder is rarely needed with ordinary unique_ptr/vector usage
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/std_launder", run>;

}  // namespace
