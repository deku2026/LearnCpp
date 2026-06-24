// LearnCpp placeholder
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : sanitizer_full_family_as_ub_detectors
// Topic id : part6/g/section05/sanitizer_full_family_as_ub_detectors
//
// TODO: read cppreference, sketch a minimal example, check godbolt / C++ Insights,
//       then replace this empty run() body with real demo code.

#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/sanitizer_full_family_as_ub_detectors", run>;

}  // namespace
