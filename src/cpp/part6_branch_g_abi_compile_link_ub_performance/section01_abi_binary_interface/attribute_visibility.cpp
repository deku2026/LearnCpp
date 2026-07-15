// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : attribute_visibility
// Topic id : part6/g/section01/attribute_visibility
//
// Covers: visibility attributes on types/functions

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

#if defined(__GNUC__) || defined(__clang__)
struct __attribute__((visibility("default"))) ApiType {
    int v = 1;
};
#else
struct ApiType {
    int v = 1;
};
#endif

void demo_basics() {
    ApiType a;
    assert(a.v == 1);
}

void demo_intermediate() {
    assert(sizeof(ApiType) >= sizeof(int));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/attribute_visibility", run>;

}  // namespace
