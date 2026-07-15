// LearnCpp topic example
// Doc      : part6-branch-a-object-model.md
// Stage    : part6_branch_a_object_model
// Section  : section05_misc
// Item     : member_pointers
// Topic id : part6/a/section05/member_pointers
//
// Covers: pointer-to-member data/function; .* and ->* operators

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

struct Widget {
    int a = 1;
    int b = 2;
    int sum() const { return a + b; }
    int scale(int k) const { return (a + b) * k; }
};

void demo_basics() {
    int Widget::* pm = &Widget::a;
    Widget w;
    assert(w.*pm == 1);
    pm = &Widget::b;
    assert(w.*pm == 2);
}

void demo_intermediate() {
    int (Widget::*pmf)() const = &Widget::sum;
    Widget w{3, 4};
    assert((w.*pmf)() == 7);
    Widget* p = &w;
    assert((p->*pmf)() == 7);
}

void demo_expert() {
    int (Widget::*pmf)(int) const = &Widget::scale;
    Widget w{5, 5};
    assert((w.*pmf)(3) == 30);
    // Member pointers are not ordinary pointers; null member pointer compares equal to 0.
    int Widget::* none = nullptr;
    assert(none == nullptr);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section05/member_pointers", run>;

}  // namespace
