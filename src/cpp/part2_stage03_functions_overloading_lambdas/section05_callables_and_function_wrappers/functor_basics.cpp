// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : functor_basics
// Topic id : part2/stage03/section05/functor_basics
//
// Covers: hand-written functor vs equivalent lambda

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <vector>

namespace {

struct Adder {
    int base;
    int operator()(int x) const { return x + base; }
};

struct Accumulator {
    int sum = 0;
    int operator()(int x) {
        sum += x;
        return sum;
    }
};

void demo_basics() {
    Adder add5{5};
    LEARN_CHECK(add5(10) == 15);

    auto lam = [base = 5](int x) { return x + base; };
    LEARN_CHECK(lam(10) == 15);
}

void demo_intermediate() {
    Accumulator acc;
    LEARN_CHECK(acc(3) == 3);
    LEARN_CHECK(acc(4) == 7);
    LEARN_CHECK(acc.sum == 7);
}

void demo_expert() {
    std::vector<int> v{3, 1, 2};
    Adder offset{10};
    std::transform(v.begin(), v.end(), v.begin(), offset);
    LEARN_CHECK(v[0] == 13 && v[1] == 11 && v[2] == 12);

    // Functors can overload operator() for multi-mode callables.
    struct Poly {
        int operator()(int x) const { return x; }
        double operator()(double x) const { return x * 0.5; }
    };
    Poly p;
    LEARN_CHECK(p(4) == 4);
    LEARN_CHECK(p(4.0) == 2.0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/functor_basics", run>;

}  // namespace
