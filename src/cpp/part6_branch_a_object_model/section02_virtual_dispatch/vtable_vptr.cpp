// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : vtable_vptr
// Topic id : part6/a/section02/vtable_vptr
//
// Covers: polymorphic objects carry vptr; virtual call dispatches via vtable

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

struct Base {
    int data = 1;
    virtual const char* name() const { return "Base"; }
    virtual int id() const { return 1; }
    virtual ~Base() = default;
};

struct Derived : Base {
    int extra = 2;
    const char* name() const override { return "Derived"; }
    // id() inherited
};

void demo_basics() {
    Base b;
    Derived d;
    assert(std::string{b.name()} == "Base");
    assert(std::string{d.name()} == "Derived");
    Base& r = d;
    assert(std::string{r.name()} == "Derived");
}

void demo_intermediate() {
    Derived d;
    Base* p = &d;
    assert(std::string{p->name()} == "Derived");
    assert(p->id() == 1);  // not overridden
    // Polymorphic types are larger than non-polymorphic equivalents.
    struct Plain {
        int data;
    };
    assert(sizeof(Base) > sizeof(Plain));
}

void demo_expert() {
    // Same call site, different dynamic type => different vtable target.
    auto call = [](const Base& o) { return std::string{o.name()}; };
    assert(call(Base{}) == "Base");
    assert(call(Derived{}) == "Derived");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/vtable_vptr", run>;

}  // namespace
