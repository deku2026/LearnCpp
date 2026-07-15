// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : object_slicing
// Topic id : part6/a/section02/object_slicing
//
// Covers: value copy to base slices derived parts; prefer pointers/references

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>

namespace {

struct Base {
    int b = 1;
    virtual std::string name() const { return "Base"; }
    virtual ~Base() = default;
};

struct Derived : Base {
    int d = 2;
    std::string name() const override { return "Derived"; }
};

void demo_basics() {
    Derived d;
    Base sliced = d;  // object slicing: copies Base subobject only
    assert(sliced.b == 1);
    assert(sliced.name() == "Base");  // dynamic type is Base
}

void demo_intermediate() {
    Derived d;
    Base& ref = d;
    Base* ptr = &d;
    assert(ref.name() == "Derived");
    assert(ptr->name() == "Derived");
}

void demo_expert() {
    // Containers of Base by value slice; use unique_ptr/shared_ptr or references.
    std::unique_ptr<Base> p = std::make_unique<Derived>();
    assert(p->name() == "Derived");
    Derived d;
    Base copy = d;
    assert(sizeof(copy) == sizeof(Base));
    (void)d.d;
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/object_slicing", run>;

}  // namespace
