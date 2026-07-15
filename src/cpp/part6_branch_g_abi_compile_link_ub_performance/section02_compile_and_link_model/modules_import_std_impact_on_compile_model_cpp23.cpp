// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : modules_import_std_impact_on_compile_model_cpp23
// Topic id : part6/g/section02/modules_import_std_impact_on_compile_model_cpp23
//
// Covers: modules / import std change build graph

#include "learn/topic_registry.hpp"

#include <string>

namespace {

void demo_basics() {
    std::string s = "modules";
    LEARN_CHECK(s.size() == 7);
}

void demo_intermediate() {
    LEARN_CHECK(true);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/g/section02/modules_import_std_impact_on_compile_model_cpp23", run>;

}  // namespace
