// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : std_launder_when_necessary
// Topic id : part6/g/section04/std_launder_when_necessary
//
// Covers: std::launder when pointers may refer to replaced objects

#include "learn/topic_registry.hpp"

#include <new>

namespace {

struct Y {
    const int n;
    explicit Y(int v) : n(v) {}
};

void demo_basics() {
    alignas(Y) unsigned char buf[sizeof(Y)];
    Y* p = new (buf) Y(1);
    LEARN_CHECK(p->n == 1);
    p->~Y();
    new (buf) Y(2);
    Y* q = std::launder(reinterpret_cast<Y*>(buf));
    LEARN_CHECK(q->n == 2);
}

void demo_intermediate() {
    int x = 3;
    LEARN_CHECK(*std::launder(&x) == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/std_launder_when_necessary", run>;

}  // namespace
