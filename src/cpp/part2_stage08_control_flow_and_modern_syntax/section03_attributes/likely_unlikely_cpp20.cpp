// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : likely_unlikely_cpp20
// Topic id : part2/stage08/section03/likely_unlikely_cpp20
//
// Covers: [[likely]] / [[unlikely]] branch hints (behavior unchanged)

#include "learn/topic_registry.hpp"

namespace {

int classify(int x) {
    if (x == 0) [[unlikely]] {
        return 0;
    } else if (x > 0) [[likely]] {
        return 1;
    } else {
        return -1;
    }
}

int clamp_nonneg(int x) {
    if (x < 0) [[unlikely]] {
        return 0;
    }
    return x;
}

void demo_basics() {
    LEARN_CHECK(classify(5) == 1);
    LEARN_CHECK(classify(-2) == -1);
    LEARN_CHECK(classify(0) == 0);
}

void demo_intermediate() {
    LEARN_CHECK(clamp_nonneg(3) == 3);
    LEARN_CHECK(clamp_nonneg(-1) == 0);
}

void demo_expert() {
    // Attributes are optimization hints; observable result is identical.
    int sum = 0;
    for (int i = 0; i < 100; ++i) {
        if (i == 50) [[unlikely]] {
            sum += 1000;
        } else [[likely]] {
            sum += 1;
        }
    }
    LEARN_CHECK(sum == 99 + 1000);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/likely_unlikely_cpp20", run>;

}  // namespace
