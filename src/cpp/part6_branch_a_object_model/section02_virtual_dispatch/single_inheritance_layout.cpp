// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : single_inheritance_layout
// Topic id : part6/a/section02/single_inheritance_layout
//
// Covers: single inheritance shares one vptr; sizeof growth from data members

#include "learn/topic_registry.hpp"

#include <cstdint>

namespace {

struct Base {
    int b = 0;
    virtual int f() const { return b; }
    virtual ~Base() = default;
};

struct Derived : Base {
    int d = 0;
    int f() const override { return b + d; }
};

void demo_basics() {
    Derived x;
    x.b = 1;
    x.d = 2;
    LEARN_CHECK(x.f() == 3);
    Base* p = &x;
    LEARN_CHECK(p->f() == 3);
}

void demo_intermediate() {
    // Single inheritance: one vptr shared; Derived adds its members.
    LEARN_CHECK(sizeof(Derived) >= sizeof(Base));
    Derived x{};
    Base* as_base = &x;
    // Primary base subobject typically at offset 0 (ABI-dependent, common case).
    LEARN_CHECK(static_cast<void*>(as_base) == static_cast<void*>(&x));
}

void demo_expert() {
    Derived x{};
    x.b = 10;
    x.d = 20;
    Base& r = x;
    LEARN_CHECK(r.f() == 30);
    LEARN_CHECK(reinterpret_cast<std::uintptr_t>(&x) % alignof(Derived) == 0);
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/single_inheritance_layout", run>;

}  // namespace
