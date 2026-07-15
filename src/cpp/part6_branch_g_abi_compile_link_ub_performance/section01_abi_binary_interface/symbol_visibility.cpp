// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : symbol_visibility
// Topic id : part6/g/section01/symbol_visibility
//
// Covers: default vs hidden visibility for shared libraries

#include "learn/topic_registry.hpp"

namespace {

#if defined(__GNUC__) || defined(__clang__)
#define EXPORT __attribute__((visibility("default")))
#define HIDDEN __attribute__((visibility("hidden")))
#else
#define EXPORT
#define HIDDEN
#endif

EXPORT int public_api() {
    return 1;
}
HIDDEN int internal_helper() {
    return 2;
}

void demo_basics() {
    LEARN_CHECK(public_api() == 1);
    LEARN_CHECK(internal_helper() == 2);
}

void demo_intermediate() {
    LEARN_CHECK(public_api() + internal_helper() == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/symbol_visibility", run>;

}  // namespace
