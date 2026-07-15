// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : symbol_resolution
// Topic id : part6/g/section02/symbol_resolution
//
// Covers: linker resolves symbols across object files

#include "learn/topic_registry.hpp"

namespace {

namespace obj_a {
int provide() {
    return 10;
}
}  // namespace obj_a
namespace obj_b {
int consume() {
    return obj_a::provide() + 1;
}
}  // namespace obj_b

void demo_basics() {
    LEARN_CHECK(obj_b::consume() == 11);
}

void demo_intermediate() {
    LEARN_CHECK(obj_a::provide() == 10);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/symbol_resolution", run>;

}  // namespace
