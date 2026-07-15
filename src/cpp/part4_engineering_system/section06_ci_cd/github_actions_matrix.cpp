// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : github_actions_matrix
// Topic id : part4/section06/github_actions_matrix
//
// Covers: GitHub Actions build matrix

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // strategy.matrix: os, compiler, build_type
    std::string os = "windows-latest";
    assert(!os.empty());
}

void demo_intermediate() {
    // fail-fast: false to see all cells
    assert(true);
}

void demo_expert() {
    // Cache vcpkg/ccache keys carefully
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/github_actions_matrix", run>;

}  // namespace
