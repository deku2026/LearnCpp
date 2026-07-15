// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : policy_based_design_idiom
// Topic id : part2/stage15/section03/policy_based_design_idiom
//
// Covers: policy-based design

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

struct AddPolicy {
    static int combine(int a, int b) { return a + b; }
};
struct MulPolicy {
    static int combine(int a, int b) { return a * b; }
};

template <class Policy>
struct Calculator {
    int apply(int a, int b) const { return Policy::combine(a, b); }
};

void demo_basics() {
    Calculator<AddPolicy> c;
    assert(c.apply(2, 3) == 5);
}

void demo_intermediate() {
    Calculator<MulPolicy> c;
    assert(c.apply(2, 3) == 6);
}

void demo_expert() {
    // Compose orthogonal policies instead of deep inheritance.
    Calculator<AddPolicy> a;
    Calculator<MulPolicy> m;
    assert(a.apply(1, 1) + m.apply(2, 2) == 6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/policy_based_design_idiom", run>;

}  // namespace
