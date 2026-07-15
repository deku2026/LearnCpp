// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : closure_class_in_insights
// Topic id : part2/stage03/section03/closure_class_in_insights
//
// Covers: lambda == unnamed closure class with operator(); hand-written twin

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

// Manual analogue of: int n = 10; auto add_n = [n](int x){ return x + n; };
struct AddN {
    int n;
    int operator()(int x) const { return x + n; }
};

void demo_basics() {
    int n = 10;
    auto add_n = [n](int x) { return x + n; };
    LEARN_CHECK(add_n(5) == 15);

    AddN manual{10};
    LEARN_CHECK(manual(5) == 15);
}

void demo_intermediate() {
    int n = 3;
    auto add_n = [n](int x) { return x + n; };
    // Distinct unique closure type per lambda expression.
    static_assert(!std::is_same_v<decltype(add_n), AddN>);

    // Default operator() is const (value captures read-only unless mutable).
    using F = decltype(add_n);
    static_assert(std::is_invocable_r_v<int, const F&, int>);
}

void demo_expert() {
    // Empty capture => conversion to function pointer.
    auto times2 = [](int x) { return x * 2; };
    int (*fp)(int) = times2;
    LEARN_CHECK(fp(21) == 42);

    int cap = 1;
    auto with_state = [cap](int x) { return x + cap; };
    LEARN_CHECK(with_state(2) == 3);
    // int (*bad)(int) = with_state; // ill-formed: has capture
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/closure_class_in_insights", run>;

}  // namespace
