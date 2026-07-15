// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : extern_c_link
// Topic id : part6/g/section01/extern_c_link
//
// Covers: extern "C" linkage for C interop

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

extern "C" int c_api_square(int x) {
    return x * x;
}

extern "C" {
int c_api_neg(int x) {
    return -x;
}
}

void demo_basics() {
    assert(c_api_square(4) == 16);
}

void demo_intermediate() {
    assert(c_api_neg(5) == -5);
}

void demo_expert() {
    using Fn = int (*)(int);
    Fn f = &c_api_square;
    assert(f(3) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/extern_c_link", run>;

}  // namespace
