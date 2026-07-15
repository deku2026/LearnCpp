// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : typeid
// Topic id : part6/a/section03/typeid
//
// Covers: typeid on polymorphic glvalues uses dynamic type; otherwise static type

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <typeinfo>

namespace {

struct Base {
    virtual ~Base() = default;
};

struct Derived : Base {};

void demo_basics() {
    assert(typeid(int) == typeid(int));
    assert(typeid(int).name() != nullptr);
}

void demo_intermediate() {
    Derived d;
    Base& r = d;
    Base* p = &d;
    assert(typeid(r) == typeid(Derived));
    assert(typeid(*p) == typeid(Derived));
    assert(typeid(p) == typeid(Base*));  // pointer expression: static type
}

void demo_expert() {
    Derived d;
    Base& r = d;
    assert(typeid(r) != typeid(Base));
    const std::type_info& ti = typeid(r);
    assert(ti == typeid(Derived));
    assert(std::string{ti.name()}.size() > 0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section03/typeid", run>;

}  // namespace
