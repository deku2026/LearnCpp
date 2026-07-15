// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : viable_functions
// Topic id : part6/d/section03/viable_functions
//
// Covers: viable functions: arity and convertibility

#include "learn/topic_registry.hpp"

#include <string>

namespace {

int g(int) {
    return 1;
}
int g(std::string) {
    return 2;
}

void demo_basics() {
    LEARN_CHECK(g(3) == 1);
    LEARN_CHECK(g("hi") == 2);
}

void demo_intermediate() {
    // g(3.0) viable via double->int conversion
    LEARN_CHECK(g(3.0) == 1);
}

void demo_expert() {
    // Too many args: not viable
    // g(1, 2); // would not compile
    LEARN_CHECK(g(std::string{"x"}) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/viable_functions", run>;

}  // namespace
