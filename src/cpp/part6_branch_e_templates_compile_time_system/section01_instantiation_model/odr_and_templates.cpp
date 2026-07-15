// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section01_instantiation_model
// Item     : odr_and_templates
// Topic id : part6/e/section01/odr_and_templates
//
// Covers: ODR for templates: one definition rule across TUs

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

template <class T>
T add_one(T x) {
    return x + T{1};
}

void demo_basics() {
    assert(add_one(1) == 2);
    assert(add_one(1.5) == 2.5);
}

void demo_intermediate() {
    // Each specialization is a separate entity; definitions must match across TUs.
    assert(add_one(2) == 3);
}

void demo_expert() {
    // Inline/header-only templates are OK if token-identical in every TU.
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section01/odr_and_templates", run>;

}  // namespace
