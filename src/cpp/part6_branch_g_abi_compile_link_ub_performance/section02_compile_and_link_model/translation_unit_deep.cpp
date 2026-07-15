// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : translation_unit_deep
// Topic id : part6/g/section02/translation_unit_deep
//
// Covers: translation unit is the compiler's unit of work

#include "learn/topic_registry.hpp"

namespace {

namespace {
int tu_local = 1;
}

int shared_func() {
    return tu_local + 1;
}

void demo_basics() {
    LEARN_CHECK(shared_func() == 2);
}

void demo_intermediate() {
    LEARN_CHECK(tu_local == 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/translation_unit_deep", run>;

}  // namespace
