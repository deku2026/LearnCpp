// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : removed_in_cpp23
// Topic id : part2/stage14/section08/removed_in_cpp23
//
// Covers: facilities removed before/around modern C++

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

void demo_basics() {
    // auto_ptr removed (C++17); use unique_ptr.
    // export templates removed long ago.
    assert(true);
}

void demo_intermediate() {
    // trigraphs removed (C++17)
    // throws-specifications dynamic exception specs removed
    assert(true);
}

void demo_expert() {
    // Garbage collector API support removed in C++23 path on many implementations.
    // Write code assuming no GC hooks.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/removed_in_cpp23", run>;

}  // namespace
