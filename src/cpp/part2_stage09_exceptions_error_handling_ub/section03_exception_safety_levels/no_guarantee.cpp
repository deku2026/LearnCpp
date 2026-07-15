// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : no_guarantee
// Topic id : part2/stage09/section03/no_guarantee
//
// Covers: no exception safety — partial updates possible (simulated, no leak)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct UnsafeAccount {
    int balance = 0;
    std::vector<std::string> log;

    void deposit_unsafe(int amount, bool fail_after_balance) {
        balance += amount;  // mutated first
        if (fail_after_balance) {
            throw std::runtime_error("fail");
        }
        log.push_back("deposit");
    }
};

void demo_basics() {
    UnsafeAccount a;
    try {
        a.deposit_unsafe(10, true);
        assert(false);
    } catch (...) {
        // No guarantee: balance already changed, log incomplete.
        assert(a.balance == 10);
        assert(a.log.empty());
    }
}

void demo_intermediate() {
    UnsafeAccount a;
    a.deposit_unsafe(5, false);
    assert(a.balance == 5);
    assert(a.log.size() == 1);
}

void demo_expert() {
    // Teaching point: callers cannot reason about object state after exception.
    UnsafeAccount a;
    a.balance = 100;
    try {
        a.deposit_unsafe(1, true);
    } catch (...) {
        assert(a.balance == 101);  // partial update remains
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/no_guarantee", run>;

}  // namespace
