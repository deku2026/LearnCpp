// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : inline_template_versus_odr
// Topic id : part6/g/section02/inline_template_versus_odr
//
// Covers: inline and templates relax ODR with identical definitions

#include "learn/topic_registry.hpp"

namespace {

template <class T>
T twice(T x) {
    return x + x;
}

inline int answer() {
    return 42;
}

void demo_basics() {
    LEARN_CHECK(twice(2) == 4);
    LEARN_CHECK(answer() == 42);
}

void demo_intermediate() {
    LEARN_CHECK(twice(1.5) == 3.0);
}

void demo_expert() {
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/inline_template_versus_odr", run>;

}  // namespace
