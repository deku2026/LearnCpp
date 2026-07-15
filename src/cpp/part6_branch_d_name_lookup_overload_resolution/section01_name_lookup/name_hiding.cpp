// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : name_hiding
// Topic id : part6/d/section01/name_hiding
//
// Covers: inner scope names hide outer names

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

struct Base {
    void f(int) {}
    int x = 1;
};

struct Derived : Base {
    void f(double) {}  // hides Base::f overloads
    int x = 2;
};

void demo_basics() {
    int x = 1;
    {
        int x = 2;
        assert(x == 2);
    }
    assert(x == 1);
}

void demo_intermediate() {
    Derived d;
    assert(d.x == 2);
    assert(d.Base::x == 1);
}

void demo_expert() {
    Derived d;
    d.f(1.0);      // Derived::f
    d.Base::f(1);  // call hidden base overload explicitly
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/name_hiding", run>;

}  // namespace
