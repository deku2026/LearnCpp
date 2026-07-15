// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_argument_dependent_lookup
// Topic id : part6/d/section01/adl_argument_dependent_lookup
//
// Covers: ADL finds functions in associated namespaces of arguments

#include "learn/topic_registry.hpp"

namespace {

namespace geo {
struct Point {
    int x, y;
};
int magnitude2(Point p) {
    return p.x * p.x + p.y * p.y;
}
}  // namespace geo

void demo_basics() {
    geo::Point p{3, 4};
    LEARN_CHECK(magnitude2(p) == 25);  // found via ADL
}

void demo_intermediate() {
    using geo::Point;
    Point p{0, 0};
    LEARN_CHECK(magnitude2(p) == 0);
}

void demo_expert() {
    // Qualified call disables ADL.
    geo::Point p{1, 0};
    LEARN_CHECK(geo::magnitude2(p) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/adl_argument_dependent_lookup", run>;

}  // namespace
