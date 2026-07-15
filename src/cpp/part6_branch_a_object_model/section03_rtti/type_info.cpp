// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : type_info
// Topic id : part6/a/section03/type_info
//
// Covers: std::type_info equality, before, name, hash_code

#include "learn/topic_registry.hpp"

#include <string>
#include <typeinfo>

namespace {

struct A {
    virtual ~A() = default;
};
struct B : A {};

void demo_basics() {
    const std::type_info& ti = typeid(int);
    LEARN_CHECK(ti == typeid(int));
    LEARN_CHECK(ti != typeid(double));
    LEARN_CHECK(ti.name() != nullptr);
}

void demo_intermediate() {
    A a;
    B b;
    LEARN_CHECK(typeid(a) == typeid(A));
    LEARN_CHECK(typeid(b) == typeid(B));
    LEARN_CHECK(typeid(a).before(typeid(b)) || typeid(b).before(typeid(a)) || typeid(a) == typeid(b));
}

void demo_expert() {
    const std::type_info& t1 = typeid(B);
    const std::type_info& t2 = typeid(B);
    LEARN_CHECK(t1 == t2);
    LEARN_CHECK(t1.hash_code() == t2.hash_code());
    LEARN_CHECK(std::string{t1.name()}.find('B') != std::string::npos || std::string{t1.name()}.size() > 0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section03/type_info", run>;

}  // namespace
