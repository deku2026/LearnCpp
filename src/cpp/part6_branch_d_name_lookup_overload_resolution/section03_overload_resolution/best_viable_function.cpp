// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : best_viable_function
// Topic id : part6/d/section03/best_viable_function
//
// Covers: selecting the best viable function

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

int h(int) {
    return 1;
}
int h(long) {
    return 2;
}
int h(double) {
    return 3;
}

void demo_basics() {
    assert(h(1) == 1);  // exact match int
}

void demo_intermediate() {
    assert(h(1L) == 2);
    assert(h(1.0) == 3);
}

void demo_expert() {
    // Ambiguous cases are ill-formed; exact match beats conversion.
    short s = 1;
    assert(h(s) == 1);  // promotion/conversion to int preferred path
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/best_viable_function", run>;

}  // namespace
