// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : default_arguments
// Topic id : part2/stage03/section01/default_arguments
//
// Covers: trailing defaults, call-site evaluation, not part of signature

#include "learn/topic_registry.hpp"

#include <string>

namespace {

// Defaults only on the declaration used at the call site (here: definition).
std::string greet(const std::string& name, const std::string& greeting = "Hi") {
    return greeting + ", " + name;
}

int scale(int value, int factor = 2, int bias = 0) {
    return value * factor + bias;
}

int eval_count = 0;
int next_default() {
    return ++eval_count;
}

int with_side_effect(int x, int y = next_default()) {
    return x + y;
}

void demo_basics() {
    LEARN_CHECK(greet("Ada") == "Hi, Ada");
    LEARN_CHECK(greet("Bob", "Yo") == "Yo, Bob");
    LEARN_CHECK(scale(5) == 10);
    LEARN_CHECK(scale(5, 3) == 15);
    LEARN_CHECK(scale(5, 3, 1) == 16);
}

void demo_intermediate() {
    // Defaults must be right-contiguous; cannot skip middle args.
    LEARN_CHECK(scale(4, 1, 0) == 4);
    // Default expressions are not part of the function type/signature.
    using Fn = int (*)(int, int, int);
    Fn fp = scale;
    LEARN_CHECK(fp(2, 5, 1) == 11);
}

void demo_expert() {
    eval_count = 0;
    LEARN_CHECK(with_side_effect(10) == 11);
    LEARN_CHECK(eval_count == 1);
    LEARN_CHECK(with_side_effect(10, 100) == 110);
    LEARN_CHECK(eval_count == 1);  // default not evaluated when arg provided
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/default_arguments", run>;

}  // namespace
