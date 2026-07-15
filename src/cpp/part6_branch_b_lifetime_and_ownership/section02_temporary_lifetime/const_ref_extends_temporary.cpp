// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : const_ref_extends_temporary
// Topic id : part6/b/section02/const_ref_extends_temporary
//
// Covers: const T& and T&& can extend temporary lifetime

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

struct Box {
    int v;
    explicit Box(int x) : v(x) {}
};

void demo_basics() {
    const Box& r = Box{7};
    LEARN_CHECK(r.v == 7);
}

void demo_intermediate() {
    Box&& rr = Box{9};
    LEARN_CHECK(rr.v == 9);
    const int& i = 5;
    LEARN_CHECK(i == 5);
}

void demo_expert() {
    // Extension binds to the reference variable's scope, not through further aliases.
    const Box& r = Box{1};
    const Box& alias = r;  // no new extension of a temporary
    LEARN_CHECK(alias.v == 1);
    LEARN_CHECK(&alias == &r);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/const_ref_extends_temporary", run>;

}  // namespace
