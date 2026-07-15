// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : recursive_lambda_via_deducing_this
// Topic id : part2/stage04/section04/recursive_lambda_via_deducing_this
//
// Covers: recursive lambda with this auto self (C++23)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>

namespace {

#if defined(__cpp_explicit_this_parameter)

void demo_basics() {
    auto fib = [](this auto self, int n) -> int { return n < 2 ? n : self(n - 1) + self(n - 2); };
    assert(fib(0) == 0);
    assert(fib(1) == 1);
    assert(fib(10) == 55);
}

void demo_intermediate() {
    auto fact = [](this const auto& self, int n) -> int { return n <= 1 ? 1 : n * self(n - 1); };
    assert(fact(5) == 120);
}

void demo_expert() {
    auto sum = [](this auto self, int n, int acc) -> int { return n == 0 ? acc : self(n - 1, acc + n); };
    assert(sum(5, 0) == 15);
}

#else

void demo_basics() {
    std::function<int(int)> fib;
    fib = [&](int n) -> int { return n < 2 ? n : fib(n - 1) + fib(n - 2); };
    assert(fib(10) == 55);
}

void demo_intermediate() {
    assert(true);
}

void demo_expert() {
    assert(true);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/recursive_lambda_via_deducing_this", run>;

}  // namespace
