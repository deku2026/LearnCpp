// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_bind_versus_lambda
// Topic id : part2/stage03/section05/std_bind_versus_lambda
//
// Covers: prefer lambda over legacy std::bind; bind_front when simple

#include "learn/topic_registry.hpp"

#include <functional>

namespace {

int sub(int a, int b) {
    return a - b;
}

void demo_basics() {
    // Preferred: lambda is clear and optimizable.
    auto f = [](int a) { return sub(a, 3); };
    LEARN_CHECK(f(10) == 7);
}

void demo_intermediate() {
    // Legacy bind with placeholders — works, harder to read.
    using std::placeholders::_1;
    auto legacy = std::bind(sub, _1, 3);
    LEARN_CHECK(legacy(10) == 7);

    auto modern = std::bind_front(sub, 10);
    LEARN_CHECK(modern(3) == 7);
}

void demo_expert() {
    // Nested logic / conditionals: lambda wins decisively.
    auto clamp_sub = [](int a, int b) {
        const int r = sub(a, b);
        return r < 0 ? 0 : r;
    };
    LEARN_CHECK(clamp_sub(2, 5) == 0);
    LEARN_CHECK(clamp_sub(9, 2) == 7);

    // bind_front/back for pure partial application only.
    auto g = std::bind_front(sub, 5);
    LEARN_CHECK(g(1) == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_versus_lambda", run>;

}  // namespace
