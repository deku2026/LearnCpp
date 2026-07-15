// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : basic_guarantee
// Topic id : part2/stage09/section03/basic_guarantee
//
// Covers: basic guarantee — no leaks, invariants hold, state may change

#include "learn/topic_registry.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

namespace {

struct BasicSafe {
    std::vector<int> data;
    int sum = 0;

    void push_recompute(int x, bool fail) {
        data.push_back(x);  // may change state
        if (fail) {
            // Keep invariant: sum matches data (recompute) even on failure path.
            sum = 0;
            for (int v : data) {
                sum += v;
            }
            throw std::runtime_error("fail");
        }
        sum += x;
    }
};

void demo_basics() {
    BasicSafe b;
    b.push_recompute(1, false);
    LEARN_CHECK(b.data.size() == 1);
    LEARN_CHECK(b.sum == 1);
}

void demo_intermediate() {
    BasicSafe b;
    try {
        b.push_recompute(5, true);
        LEARN_CHECK(false);
    } catch (...) {
        // Invariant holds even though operation failed.
        LEARN_CHECK(b.data.size() == 1);
        LEARN_CHECK(b.sum == 5);
    }
}

void demo_expert() {
    // RAII ensures no resource leak under basic guarantee.
    try {
        auto p = std::make_unique<int>(1);
        throw std::runtime_error("x");
    } catch (...) {
        // p destroyed; no leak
    }
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/basic_guarantee", run>;

}  // namespace
