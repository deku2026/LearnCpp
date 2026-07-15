// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : using_introduction
// Topic id : part6/d/section01/using_introduction
//
// Covers: using-declaration brings names into scope

#include "learn/topic_registry.hpp"

namespace {

struct Base {
    void f(int) {}
    void f(double) {}
};

struct Derived : Base {
    using Base::f;
    void f(char) {}
};

namespace a {
int g = 1;
}
namespace b {
using a::g;
}

void demo_basics() {
    LEARN_CHECK(b::g == 1);
}

void demo_intermediate() {
    Derived d;
    d.f(1);    // Base::f(int) via using
    d.f(1.0);  // Base::f(double)
    d.f('x');  // Derived::f(char)
    LEARN_CHECK(true);
}

void demo_expert() {
    using std_size = decltype(sizeof(0));
    std_size n = 3;
    LEARN_CHECK(n == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/using_introduction", run>;

}  // namespace
