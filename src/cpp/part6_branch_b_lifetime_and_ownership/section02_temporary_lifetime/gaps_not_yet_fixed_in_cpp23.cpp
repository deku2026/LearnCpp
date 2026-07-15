// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : gaps_not_yet_fixed_in_cpp23
// Topic id : part6/b/section02/gaps_not_yet_fixed_in_cpp23
//
// Covers: remaining lifetime footguns; safe patterns only

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

std::string make_s() {
    return "data";
}

// UNSAFE pattern (not executed): string_view sv = make_s(); use sv after full-expression.

void demo_basics() {
    std::string owner = make_s();
    std::string_view sv = owner;
    assert(sv == "data");
}

void demo_intermediate() {
    // Keep owning string alive as long as views need it.
    const std::string& ext = make_s();
    std::string_view sv = ext;
    assert(sv.size() == 4);
}

void demo_expert() {
    // Comment-only gap examples:
    // - chaining getters that return references into temporaries
    // - init-captures that dangle
    // Prefer owning types or extend lifetime at the call site.
    std::string s = make_s();
    assert(!s.empty());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section02/gaps_not_yet_fixed_in_cpp23", run>;

}  // namespace
