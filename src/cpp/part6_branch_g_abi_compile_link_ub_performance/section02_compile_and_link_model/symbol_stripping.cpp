// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : symbol_stripping
// Topic id : part6/g/section02/symbol_stripping
//
// Covers: unused symbol stripping / reduce export set

#include "learn/topic_registry.hpp"

namespace {

static int only_here() {
    return 7;
}

int keep_api() {
    return only_here();
}

void demo_basics() {
    LEARN_CHECK(keep_api() == 7);
}

void demo_intermediate() {
    LEARN_CHECK(only_here() == 7);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/symbol_stripping", run>;

}  // namespace
