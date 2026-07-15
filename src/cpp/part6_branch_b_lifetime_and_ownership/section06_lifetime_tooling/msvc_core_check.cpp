// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : msvc_core_check
// Topic id : part6/b/section06/msvc_core_check
//
// Covers: MSVC code analysis / C++ Core Check friendly patterns

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v(3);
    for (std::size_t i = 0; i < v.size(); ++i) {
        v[i] = static_cast<int>(i);
    }
    assert(v[2] == 2);
}

void demo_intermediate() {
    auto p = std::make_unique<int[]>(4);
    p[0] = 1;
    assert(p[0] == 1);
}

void demo_expert() {
    // Prefer at() or gsl::at in audited code paths.
    std::vector<int> v{9};
    assert(v.at(0) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/msvc_core_check", run>;

}  // namespace
