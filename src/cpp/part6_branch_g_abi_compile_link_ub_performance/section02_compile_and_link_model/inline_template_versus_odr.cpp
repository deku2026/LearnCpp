// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : inline_template_versus_odr
// Topic id : part6/g/section02/inline_template_versus_odr
//
// Covers: inline and templates relax ODR with identical definitions

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

template <class T>
T twice(T x) {
    return x + x;
}

inline int answer() {
    return 42;
}

void demo_basics() {
    assert(twice(2) == 4);
    assert(answer() == 42);
}

void demo_intermediate() {
    assert(twice(1.5) == 3.0);
}

void demo_expert() {
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/inline_template_versus_odr", run>;

}  // namespace
