// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : multiple_inheritance_multiple_vptr
// Topic id : part6/a/section02/multiple_inheritance_multiple_vptr
//
// Covers: multiple polymorphic bases, pointer adjustment, multiple vptrs (conceptual)

#include "learn/topic_registry.hpp"

#include <cstdint>

namespace {

struct Base1 {
    int b1 = 1;
    virtual int f1() const { return b1; }
    virtual ~Base1() = default;
};

struct Base2 {
    int b2 = 2;
    virtual int f2() const { return b2; }
    virtual ~Base2() = default;
};

struct Derived : Base1, Base2 {
    int d = 3;
    int f1() const override { return b1 + d; }
    int f2() const override { return b2 + d; }
};

void demo_basics() {
    Derived x;
    LEARN_CHECK(x.f1() == 4);
    LEARN_CHECK(x.f2() == 5);
}

void demo_intermediate() {
    Derived x;
    Base1* p1 = &x;
    Base2* p2 = &x;
    LEARN_CHECK(p1->f1() == 4);
    LEARN_CHECK(p2->f2() == 5);
    // Conversion to non-primary base may change the address (ABI-dependent).
    const auto a0 = reinterpret_cast<std::uintptr_t>(&x);
    const auto a2 = reinterpret_cast<std::uintptr_t>(p2);
    LEARN_CHECK(a2 == a0 || a2 > a0);
}

void demo_expert() {
    Derived x;
    Base2* p2 = &x;
    Derived* back = static_cast<Derived*>(p2);
    LEARN_CHECK(back == &x);
    // reinterpret_cast would NOT adjust; do not use it for hierarchy casts.
    LEARN_CHECK(sizeof(Derived) >= sizeof(Base1) + sizeof(int));
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/multiple_inheritance_multiple_vptr", run>;

}  // namespace
