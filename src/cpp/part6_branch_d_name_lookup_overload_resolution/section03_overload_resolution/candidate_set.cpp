// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : candidate_set
// Topic id : part6/d/section03/candidate_set
//
// Covers: forming the candidate set for overload resolution

#include "learn/topic_registry.hpp"

namespace {

int f(int) {
    return 1;
}
int f(double) {
    return 2;
}
int f(int, int) {
    return 3;
}

void demo_basics() {
    LEARN_CHECK(f(1) == 1);
    LEARN_CHECK(f(1.0) == 2);
}

void demo_intermediate() {
    LEARN_CHECK(f(1, 2) == 3);
}

void demo_expert() {
    // Candidates include functions found by ordinary lookup + ADL.
    LEARN_CHECK(f(static_cast<short>(1)) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/candidate_set", run>;

}  // namespace
