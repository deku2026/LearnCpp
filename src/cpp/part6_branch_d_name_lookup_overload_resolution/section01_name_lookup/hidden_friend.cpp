// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : hidden_friend
// Topic id : part6/d/section01/hidden_friend
//
// Covers: hidden friends only found by ADL

#include "learn/topic_registry.hpp"

#include <sstream>

namespace {

struct Hidden {
    int v = 0;
    friend int get(const Hidden& h) { return h.v; }
    friend bool operator==(const Hidden& a, const Hidden& b) { return a.v == b.v; }
};

void demo_basics() {
    Hidden h{4};
    LEARN_CHECK(get(h) == 4);
}

void demo_intermediate() {
    Hidden a{1}, b{1}, c{2};
    LEARN_CHECK(a == b);
    LEARN_CHECK(!(a == c));
}

void demo_expert() {
    // get is not in ordinary unqualified lookup without an argument of type Hidden.
    Hidden h{9};
    LEARN_CHECK(get(h) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/hidden_friend", run>;

}  // namespace
