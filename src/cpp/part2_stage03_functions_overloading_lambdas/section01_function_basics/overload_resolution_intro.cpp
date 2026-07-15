// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : overload_resolution_intro
// Topic id : part2/stage03/section01/overload_resolution_intro
//
// Covers: overload set, ranking (exact > promotion > conversion), traps

#include "learn/topic_registry.hpp"

#include <string>

namespace {

enum class Kind { Int, Double, CStr, Long };

Kind which(int) {
    return Kind::Int;
}
Kind which(double) {
    return Kind::Double;
}
Kind which(const char*) {
    return Kind::CStr;
}

int score(int x) {
    return x;
}
int score(int x, int y) {
    return x + y;
}

void demo_basics() {
    LEARN_CHECK(which(42) == Kind::Int);
    LEARN_CHECK(which(3.14) == Kind::Double);
    LEARN_CHECK(which("text") == Kind::CStr);
}

void demo_intermediate() {
    // char promotes to int better than converting to double
    LEARN_CHECK(which('a') == Kind::Int);
    LEARN_CHECK(which(true) == Kind::Int);

    LEARN_CHECK(score(3) == 3);
    LEARN_CHECK(score(3, 4) == 7);
    // Return type alone cannot overload: int f(); double f(); is ill-formed.
}

void demo_expert() {
    // Default args + overload can create ambiguity; keep sets disjoint.
    // void g(int); void g(int, int = 0);  // g(5) would be ambiguous
    LEARN_CHECK(score(1) == 1);
    LEARN_CHECK(score(1, 0) == 1);

    const char* p = nullptr;
    LEARN_CHECK(which(p) == Kind::CStr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/overload_resolution_intro", run>;

}  // namespace
