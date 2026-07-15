// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : build_type_matrix_debug_release_relwithdebinfo
// Topic id : part4/section06/build_type_matrix_debug_release_relwithdebinfo
//
// Covers: Debug/Release/RelWithDebInfo CI cells

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Debug: asserts; Release: optimize; RelWithDebInfo: profile/prod symbols
    std::string types[] = {"Debug", "Release", "RelWithDebInfo"};
    assert(types[0] == "Debug");
}

void demo_intermediate() {
    // Some bugs only appear with optimization
    assert(true);
}

void demo_expert() {
    // Ship RelWithDebInfo + separate debug info when possible
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/build_type_matrix_debug_release_relwithdebinfo", run>;

}  // namespace
