// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_by_value
// Topic id : part2/stage03/section03/capture_by_value
//
// Covers: [x], [=], mixed defaults; copy is independent

#include "learn/topic_registry.hpp"

#include <string>

namespace {

void demo_basics() {
    int a = 1;
    auto f = [a] { return a; };
    a = 99;
    LEARN_CHECK(f() == 1);
}

void demo_intermediate() {
    int a = 2;
    int b = 3;
    auto all = [=] { return a + b; };
    a = 0;
    b = 0;
    LEARN_CHECK(all() == 5);

    int x = 10;
    int y = 20;
    auto mixed = [=, &y] { return x + y; };
    x = 0;
    y = 1;
    LEARN_CHECK(mixed() == 11);  // x copy=10, y live=1
}

void demo_expert() {
    std::string s = "copy-me";
    auto f = [s] { return s.size(); };
    s.clear();
    LEARN_CHECK(f() == 7);

    // Prefer value capture when storing/returning callables across scopes.
    auto make = [] {
        int local = 42;
        return [local] { return local; };
    };
    auto g = make();
    LEARN_CHECK(g() == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_value", run>;

}  // namespace
