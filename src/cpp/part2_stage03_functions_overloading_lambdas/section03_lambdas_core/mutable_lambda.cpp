// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : mutable_lambda
// Topic id : part2/stage03/section03/mutable_lambda
//
// Covers: default const operator(); mutable mutates the copy only

#include "learn/topic_registry.hpp"

namespace {

void demo_basics() {
    int n = 0;
    auto counter = [n]() mutable { return ++n; };
    LEARN_CHECK(counter() == 1);
    LEARN_CHECK(counter() == 2);
    LEARN_CHECK(counter() == 3);
    LEARN_CHECK(n == 0);
}

void demo_intermediate() {
    int n = 5;
    // Without mutable, value-captured n is const inside operator().
    auto read = [n] { return n; };
    LEARN_CHECK(read() == 5);

    auto bump_outer = [&n] { ++n; };
    bump_outer();
    LEARN_CHECK(n == 6);
}

void demo_expert() {
    int seed = 1;
    auto a = [seed]() mutable {
        seed *= 2;
        return seed;
    };
    auto b = a;  // copy of closure state
    LEARN_CHECK(a() == 2);
    LEARN_CHECK(a() == 4);
    LEARN_CHECK(b() == 2);  // independent copy started at seed=1
    LEARN_CHECK(seed == 1);
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
