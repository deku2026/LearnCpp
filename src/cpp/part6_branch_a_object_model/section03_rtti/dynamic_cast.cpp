// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : dynamic_cast
// Topic id : part6/a/section03/dynamic_cast
//
// Covers: safe downcast/cross-cast with RTTI; nullptr or bad_cast on failure

#include "learn/topic_registry.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace {

struct Base {
    virtual ~Base() = default;
    virtual int tag() const { return 0; }
};

struct Derived : Base {
    int tag() const override { return 1; }
    int extra() const { return 99; }
};

struct Other : Base {
    int tag() const override { return 2; }
};

void demo_basics() {
    Derived d;
    Base* p = &d;
    Derived* ok = dynamic_cast<Derived*>(p);
    assert(ok != nullptr);
    assert(ok->extra() == 99);
}

void demo_intermediate() {
    Other o;
    Base* p = &o;
    Derived* fail = dynamic_cast<Derived*>(p);
    assert(fail == nullptr);
    try {
        Base& r = o;
        (void)dynamic_cast<Derived&>(r);
        assert(false);
    } catch (const std::bad_cast&) {
        // expected
    }
}

void demo_expert() {
    Derived d;
    Base* p = &d;
    void* complete = dynamic_cast<void*>(p);
    assert(complete == static_cast<void*>(&d));
    assert(dynamic_cast<Base*>(p)->tag() == 1);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section03/dynamic_cast", run>;

}  // namespace
