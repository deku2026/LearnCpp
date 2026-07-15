// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : one_definition_rule_deep
// Topic id : part6/g/section02/one_definition_rule_deep
//
// Covers: ODR: one definition of non-inline entities in the program

#include "learn/topic_registry.hpp"

namespace {

inline int odr_ok() {
    return 5;
}

struct Widget {
    int x = 1;
    int get() const { return x; }
};

void demo_basics() {
    LEARN_CHECK(odr_ok() == 5);
}

void demo_intermediate() {
    Widget w;
    LEARN_CHECK(w.get() == 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/one_definition_rule_deep", run>;

}  // namespace
