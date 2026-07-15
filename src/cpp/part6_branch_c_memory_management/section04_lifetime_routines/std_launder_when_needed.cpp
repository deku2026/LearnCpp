// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : std_launder_when_needed
// Topic id : part6/c/section04/std_launder_when_needed
//
// Covers: std::launder after reusing storage with new object of same type

#include "learn/topic_registry.hpp"

#include <cassert>
#include <new>

namespace {

struct X {
    const int n;
    explicit X(int v) : n(v) {}
};

void demo_basics() {
    alignas(X) unsigned char buf[sizeof(X)];
    X* p = new (buf) X(1);
    assert(p->n == 1);
    p->~X();
    p = new (buf) X(2);
    assert(std::launder(p)->n == 2 || p->n == 2);
}

void demo_intermediate() {
    // launder needed when a pointer may still "see" the old object's const members
    // under the abstract machine after storage reuse.
    alignas(X) unsigned char buf[sizeof(X)];
    X* p = new (buf) X(3);
    p->~X();
    new (buf) X(4);
    X* q = std::launder(reinterpret_cast<X*>(buf));
    assert(q->n == 4);
}

void demo_expert() {
    int x = 1;
    int* p = &x;
    assert(*std::launder(p) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/std_launder_when_needed", run>;

}  // namespace
