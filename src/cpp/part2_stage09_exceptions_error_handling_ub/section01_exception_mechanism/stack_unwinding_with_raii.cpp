// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : stack_unwinding_with_raii
// Topic id : part2/stage09/section01/stack_unwinding_with_raii
//
// Covers: stack unwinding runs destructors (RAII cleanup)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

struct Guard {
    int& counter;
    explicit Guard(int& c) : counter(c) { ++counter; }
    ~Guard() { --counter; }
    Guard(const Guard&) = delete;
    Guard& operator=(const Guard&) = delete;
};

void may_throw(bool do_throw) {
    if (do_throw) {
        throw std::runtime_error("boom");
    }
}

void demo_basics() {
    int live = 0;
    try {
        Guard g(live);
        assert(live == 1);
        may_throw(true);
    } catch (const std::runtime_error&) {
        assert(live == 0);  // destructor ran during unwind
    }
    assert(live == 0);
}

void demo_intermediate() {
    int live = 0;
    try {
        Guard a(live);
        Guard b(live);
        assert(live == 2);
        may_throw(true);
    } catch (...) {
        assert(live == 0);
    }
}

void demo_expert() {
    int live = 0;
    {
        Guard g(live);
        assert(live == 1);
        may_throw(false);
    }
    assert(live == 0);

    // Nested scopes still unwind innermost first.
    int order = 0;
    struct Ordered {
        int& o;
        int id;
        ~Ordered() { o = o * 10 + id; }
    };
    try {
        Ordered a{order, 1};
        Ordered b{order, 2};
        throw 1;
    } catch (...) {
        // b then a: order becomes 2 then 21
        assert(order == 21);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/stack_unwinding_with_raii", run>;

}  // namespace
