// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : attributes_on_lambda_cpp23
// Topic id : part2/stage03/section04/attributes_on_lambda_cpp23
//
// Covers: attributes on lambda call operator (e.g. [[nodiscard]])

#include "learn/topic_registry.hpp"

namespace {

void demo_basics() {
#if defined(__cplusplus) && __cplusplus >= 202302L
    auto make = [] [[nodiscard]] () { return 42; };
#else
    auto make = []() -> int { return 42; };
#endif
    LEARN_CHECK(make() == 42);
}

void demo_intermediate() {
#if defined(__cplusplus) && __cplusplus >= 202302L
    auto scale = [] [[nodiscard]] (int x) { return x * 2; };
#else
    auto scale = [](int x) { return x * 2; };
#endif
    LEARN_CHECK(scale(5) == 10);
}

void demo_expert() {
    // Attributes document intent; discarding still compiles for non-nodiscard paths.
    int sink = 0;
#if defined(__cplusplus) && __cplusplus >= 202302L
    auto touch = [&sink] [[maybe_unused]] () { ++sink; };
#else
    auto touch = [&sink]() { ++sink; };
#endif
    touch();
    LEARN_CHECK(sink == 1);

    auto id = [](int x) noexcept { return x; };
    LEARN_CHECK(id(3) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/attributes_on_lambda_cpp23", run>;

}  // namespace
