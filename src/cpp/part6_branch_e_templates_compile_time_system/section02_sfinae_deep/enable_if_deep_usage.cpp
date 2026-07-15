// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : enable_if_deep_usage
// Topic id : part6/e/section02/enable_if_deep_usage
//
// Covers: enable_if for constrained overloads

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
int classify(T) {
    return 1;
}

template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
int classify(T) {
    return 2;
}

void demo_basics() {
    assert(classify(1) == 1);
    assert(classify(1.0) == 2);
}

void demo_intermediate() {
    assert(classify(true) == 1);
    assert(classify(1.0f) == 2);
}

void demo_expert() {
    // Prefer concepts in C++20+, enable_if still appears in older code.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/enable_if_deep_usage", run>;

}  // namespace
