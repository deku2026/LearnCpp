// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : uninitialized_read
// Topic id : part2/stage09/section08/uninitialized_read
//
// Covers: reading uninitialized values is UB — always initialize

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>
#include <vector>

namespace {

struct Point {
    int x = 0;  // default member init
    int y = 0;
};

void demo_basics() {
    int x = 0;  // SAFE
    assert(x == 0);
    Point p;
    assert(p.x == 0 && p.y == 0);
}

void demo_intermediate() {
    std::vector<int> v(3);  // value-initialized to 0
    assert(v[0] == 0 && v[2] == 0);

    std::optional<int> o;
    assert(!o.has_value());  // empty, not an uninitialized int read
    o = 5;
    assert(*o == 5);
}

void demo_expert() {
    // Comment only: `int x; int y = x;` is UB if x is automatic and unread init.
    int a{};
    int b{1};
    assert(a == 0);
    assert(b == 1);

    char buf[4]{};
    assert(buf[0] == '\0');
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/uninitialized_read", run>;

}  // namespace
