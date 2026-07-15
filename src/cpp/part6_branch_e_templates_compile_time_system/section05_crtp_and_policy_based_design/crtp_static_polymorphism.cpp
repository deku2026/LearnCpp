// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : crtp_static_polymorphism
// Topic id : part6/e/section05/crtp_static_polymorphism
//
// Covers: CRTP static polymorphism

#include "learn/topic_registry.hpp"

namespace {

template <class D>
struct Counter {
    int count() const { return static_cast<const D*>(this)->size_impl(); }
};

struct Vec : Counter<Vec> {
    int n = 3;
    int size_impl() const { return n; }
};

void demo_basics() {
    Vec v;
    LEARN_CHECK(v.count() == 3);
}

void demo_intermediate() {
    Vec v;
    v.n = 10;
    LEARN_CHECK(v.count() == 10);
}

void demo_expert() {
    const Counter<Vec>& c = Vec{};
    LEARN_CHECK(c.count() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/crtp_static_polymorphism", run>;

}  // namespace
