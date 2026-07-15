// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : static_library_versus_dynamic_library
// Topic id : part6/g/section02/static_library_versus_dynamic_library
//
// Covers: static archives vs shared libraries (conceptual)

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

int static_style_add(int a, int b) {
    return a + b;
}

void demo_basics() {
    assert(static_style_add(2, 3) == 5);
}

void demo_intermediate() {
    assert(true);
}

void demo_expert() {
    using Fn = int (*)(int, int);
    Fn f = &static_style_add;
    assert(f(1, 1) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/static_library_versus_dynamic_library", run>;

}  // namespace
