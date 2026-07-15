// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : scope_guard_rollback
// Topic id : part2/stage09/section07/scope_guard_rollback
//
// Covers: scope_guard pattern for rollback on failure

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <stdexcept>
#include <utility>

namespace {

class ScopeGuard {
    std::function<void()> fn_;
    bool active_ = true;

public:
    explicit ScopeGuard(std::function<void()> fn) : fn_(std::move(fn)) {}
    ~ScopeGuard() {
        if (active_ && fn_) {
            fn_();
        }
    }
    void dismiss() noexcept { active_ = false; }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

void demo_basics() {
    int x = 0;
    {
        ScopeGuard g([&] { x = 1; });
    }
    assert(x == 1);
}

void demo_intermediate() {
    int x = 0;
    {
        ScopeGuard g([&] { x = 99; });
        g.dismiss();
    }
    assert(x == 0);
}

void demo_expert() {
    int balance = 100;
    try {
        balance -= 30;
        ScopeGuard rollback([&] { balance += 30; });
        throw std::runtime_error("fail");
        rollback.dismiss();
    } catch (...) {
        assert(balance == 100);
    }

    balance = 100;
    {
        balance -= 10;
        ScopeGuard rollback([&] { balance += 10; });
        // success path
        rollback.dismiss();
    }
    assert(balance == 90);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section07/scope_guard_rollback", run>;

}  // namespace
