// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_by_reference
// Topic id : part2/stage03/section03/capture_by_reference
//
// Covers: [&x], [&], mixed; dangling risk (demonstrated safely)

#include "learn/topic_registry.hpp"

#include <functional>
#include <vector>

namespace {

void demo_basics() {
    int a = 1;
    auto f = [&a] { return a; };
    a = 99;
    LEARN_CHECK(f() == 99);

    auto inc = [&a] { ++a; };
    inc();
    LEARN_CHECK(a == 100);
}

void demo_intermediate() {
    int a = 1;
    int b = 2;
    auto all = [&] { return a + b; };
    a = 10;
    b = 20;
    LEARN_CHECK(all() == 30);

    auto mixed = [&, a] { return a + b; };  // a by value, b by ref
    a = 0;
    b = 5;
    LEARN_CHECK(mixed() == 15);  // captured a was 10, b is 5
}

void demo_expert() {
    // Safe: lambda used while locals live (sort, algorithm callbacks).
    std::vector<int> v{3, 1, 2};
    int threshold = 2;
    int count = 0;
    for (int x : v) {
        if (x > threshold) {
            ++count;
        }
    }
    LEARN_CHECK(count == 1);

    // Dangling avoided: value-capture when returning callable.
    auto good = [] {
        int local = 42;
        return std::function<int()>{[local] { return local; }};
    };
    LEARN_CHECK(good()() == 42);

    // BAD pattern (not executed): return [&local]{ return local; }; // UB after return
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_reference", run>;

}  // namespace
