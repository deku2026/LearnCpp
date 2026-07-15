// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : init_capture_cpp14
// Topic id : part2/stage03/section03/init_capture_cpp14
//
// Covers: [x = expr], move unique_ptr into closure

#include "learn/topic_registry.hpp"

#include <memory>
#include <utility>

namespace {

void demo_basics() {
    int a = 3;
    auto twice = [x = a * 2] { return x; };
    LEARN_CHECK(twice() == 6);
}

void demo_intermediate() {
    auto p = std::make_unique<int>(42);
    auto owns = [ptr = std::move(p)] { return *ptr; };
    LEARN_CHECK(p == nullptr);
    LEARN_CHECK(owns() == 42);
}

void demo_expert() {
    auto p = std::make_unique<int>(7);
    auto f = [ptr = std::move(p)]() mutable {
        *ptr += 1;
        return *ptr;
    };
    LEARN_CHECK(f() == 8);
    LEARN_CHECK(f() == 9);

    // Recursive-style counter via init-capture state (no self-call needed).
    auto step = [n = 0]() mutable { return ++n; };
    LEARN_CHECK(step() == 1);
    LEARN_CHECK(step() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/init_capture_cpp14", run>;

}  // namespace
