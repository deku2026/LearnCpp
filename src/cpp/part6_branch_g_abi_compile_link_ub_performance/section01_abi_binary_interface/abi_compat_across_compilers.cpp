// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : abi_compat_across_compilers
// Topic id : part6/g/section01/abi_compat_across_compilers
//
// Covers: ABI compatibility is not guaranteed across compilers/versions

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct Layout {
    char c;
    int i;
};

void demo_basics() {
    assert(sizeof(Layout) >= sizeof(int));
    assert(alignof(Layout) >= alignof(int));
}

void demo_intermediate() {
    static_assert(std::is_standard_layout_v<Layout>);
    Layout a{'x', 1};
    assert(a.i == 1);
}

void demo_expert() {
    Layout a{'a', 2};
    assert(a.c == 'a');
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/abi_compat_across_compilers", run>;

}  // namespace
