// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_function
// Topic id : part2/stage03/section05/std_function
//
// Covers: type-erased copyable callable wrapper; empty call throws

#include "learn/topic_registry.hpp"

#include <functional>
#include <stdexcept>
#include <vector>

namespace {

int free_inc(int x) {
    return x + 1;
}

void demo_basics() {
    std::function<int(int)> f = [](int x) { return x + 1; };
    LEARN_CHECK(f(10) == 11);

    f = free_inc;
    LEARN_CHECK(f(10) == 11);
}

void demo_intermediate() {
    int base = 100;
    std::function<int(int)> f = [base](int x) { return x + base; };
    LEARN_CHECK(f(10) == 110);

    std::vector<std::function<int(int)>> handlers;
    handlers.push_back([](int x) { return x * 2; });
    handlers.push_back([](int x) { return x - 1; });
    LEARN_CHECK(handlers[0](5) == 10);
    LEARN_CHECK(handlers[1](5) == 4);
}

void demo_expert() {
    std::function<int(int)> empty;
    LEARN_CHECK(!empty);
    bool threw = false;
    try {
        (void)empty(1);
    } catch (const std::bad_function_call&) {
        threw = true;
    }
    LEARN_CHECK(threw);

    // Requires copyable target; move-only lambdas need move_only_function.
    std::function<int()> ok = [] { return 1; };
    auto copy = ok;
    LEARN_CHECK(copy() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_function", run>;

}  // namespace
