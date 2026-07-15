// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : typeid
// Topic id : part6/a/section03/typeid
//
// Covers: typeid on polymorphic glvalues uses dynamic type; otherwise static type

#include "learn/topic_registry.hpp"

#include <string>
#include <typeinfo>

namespace {

struct Base {
    virtual ~Base() = default;
};

struct Derived : Base {};

void demo_basics() {
    LEARN_CHECK(typeid(int) == typeid(int));
    LEARN_CHECK(typeid(int).name() != nullptr);
}

void demo_intermediate() {
    Derived d;
    Base& r = d;
    Base* p = &d;
    LEARN_CHECK(typeid(r) == typeid(Derived));
    LEARN_CHECK(typeid(*p) == typeid(Derived));
    LEARN_CHECK(typeid(p) == typeid(Base*));  // pointer expression: static type
}

void demo_expert() {
    Derived d;
    Base& r = d;
    LEARN_CHECK(typeid(r) != typeid(Base));
    const std::type_info& ti = typeid(r);
    LEARN_CHECK(ti == typeid(Derived));
    LEARN_CHECK(std::string{ti.name()}.size() > 0);
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
