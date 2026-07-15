// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : dangling_reference
// Topic id : part2/stage09/section08/dangling_reference
//
// Covers: dangling refs/pointers — return by value, extend lifetime correctly

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace {

// BAD pattern (not called): const int& bad() { int x = 1; return x; }

int by_value() {
    int x = 42;
    return x;  // SAFE
}

const std::string& pick(const std::string& a, const std::string& b) {
    return a.size() >= b.size() ? a : b;  // refs must outlive call
}

void demo_basics() {
    assert(by_value() == 42);
}

void demo_intermediate() {
    std::string a = "hi";
    std::string b = "hello";
    const std::string& r = pick(a, b);
    assert(r == "hello");
    assert(b == "hello");
}

void demo_expert() {
    // Temporary lifetime: const ref extends temporary to full expression / ref lifetime.
    const std::string& t = std::string{"tmp"};
    assert(t == "tmp");

    std::vector<int> v{1, 2, 3};
    const int& e = v[1];
    assert(e == 2);
    // Comment only: holding ref into vector then reallocate may dangle — avoid.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/dangling_reference", run>;

}  // namespace
