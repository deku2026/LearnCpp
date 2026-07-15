// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_reference
// Topic id : part6/b/section03/dangling_reference
//
// Covers: do not return references to locals; safe return by value

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

// int& bad() { int x = 1; return x; } // dangling — not called

int safe_value() {
    int x = 1;
    return x;
}

const std::string& pick(const std::string& a, const std::string& b) {
    return a.size() >= b.size() ? a : b;
}

void demo_basics() {
    assert(safe_value() == 1);
}

void demo_intermediate() {
    std::string a = "hi";
    std::string b = "hello";
    const std::string& r = pick(a, b);
    assert(r == "hello");
}

void demo_expert() {
    // Returning ref to parameter is OK only while arguments live.
    std::string a = "x";
    std::string b = "yy";
    assert(pick(a, b) == "yy");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_reference", run>;

}  // namespace
