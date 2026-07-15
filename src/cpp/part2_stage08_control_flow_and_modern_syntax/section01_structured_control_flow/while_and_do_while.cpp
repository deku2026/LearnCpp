// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : while_and_do_while
// Topic id : part2/stage08/section01/while_and_do_while
//
// Covers: while vs do-while (execute-at-least-once)

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

int countdown_sum(int n) {
    int s = 0;
    while (n > 0) {
        s += n;
        --n;
    }
    return s;
}

int do_while_count(int n) {
    int c = 0;
    do {
        ++c;
        --n;
    } while (n > 0);
    return c;
}

void demo_basics() {
    assert(countdown_sum(5) == 15);
    assert(countdown_sum(0) == 0);
}

void demo_intermediate() {
    assert(do_while_count(3) == 3);
    // do-while runs once even when condition starts false after first body.
    assert(do_while_count(0) == 1);  // n becomes -1 after first iteration
}

void demo_expert() {
    // Euclid GCD with while.
    auto gcd = [](int a, int b) {
        while (b != 0) {
            int t = a % b;
            a = b;
            b = t;
        }
        return a;
    };
    assert(gcd(48, 18) == 6);
    assert(gcd(7, 3) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/while_and_do_while", run>;

}  // namespace
