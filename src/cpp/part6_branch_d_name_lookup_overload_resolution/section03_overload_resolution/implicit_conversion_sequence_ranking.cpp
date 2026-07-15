// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : implicit_conversion_sequence_ranking
// Topic id : part6/d/section03/implicit_conversion_sequence_ranking
//
// Covers: ICS ranking: exact > promotion > conversion

#include "learn/topic_registry.hpp"

namespace {

int k(int) {
    return 1;
}
int k(double) {
    return 2;
}

void demo_basics() {
    LEARN_CHECK(k(1) == 1);    // exact
    LEARN_CHECK(k(1.0) == 2);  // exact double
}

void demo_intermediate() {
    LEARN_CHECK(k(true) == 1);  // promotion bool->int
}

void demo_expert() {
    char c = 1;
    LEARN_CHECK(k(c) == 1);  // promotion to int
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/implicit_conversion_sequence_ranking", run>;

}  // namespace
