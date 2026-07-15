// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : undeprecation_and_semantic_changes
// Topic id : part2/stage14/section08/undeprecation_and_semantic_changes
//
// Covers: undeprecations / semantic tightenings

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

void demo_basics() {
    // Some older features were undeprecated or re-specified with clearer semantics.
    // Example: volatile still exists but is not for threading.
    volatile int v = 1;
    assert(v == 1);
}

void demo_intermediate() {
    // Prefer feature-test macros to detect semantic availability.
#if defined(__cpp_lib_constexpr_algorithms)
    assert(true);
#else
    assert(true);
#endif
}

void demo_expert() {
    static_assert(std::is_same_v<void, void>);
    // Track defect reports via feature macros rather than compiler version alone.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section08/undeprecation_and_semantic_changes", run>;

}  // namespace
