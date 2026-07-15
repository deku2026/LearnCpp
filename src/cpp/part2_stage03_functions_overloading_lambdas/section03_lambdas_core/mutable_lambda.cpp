// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : mutable_lambda
// Topic id : part2/stage03/section03/mutable_lambda
//
// Covers: default const operator(); mutable mutates the copy only

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

void demo_basics() {
    int n = 0;
    auto counter = [n]() mutable { return ++n; };
    assert(counter() == 1);
    assert(counter() == 2);
    assert(counter() == 3);
    assert(n == 0);
}

void demo_intermediate() {
    int n = 5;
    // Without mutable, value-captured n is const inside operator().
    auto read = [n] { return n; };
    assert(read() == 5);

    auto bump_outer = [&n] { ++n; };
    bump_outer();
    assert(n == 6);
}

void demo_expert() {
    int seed = 1;
    auto a = [seed]() mutable {
        seed *= 2;
        return seed;
    };
    auto b = a;  // copy of closure state
    assert(a() == 2);
    assert(a() == 4);
    assert(b() == 2);  // independent copy started at seed=1
    assert(seed == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/mutable_lambda", run>;

}  // namespace
