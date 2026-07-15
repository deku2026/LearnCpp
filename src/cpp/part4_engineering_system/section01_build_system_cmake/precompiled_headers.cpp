// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : precompiled_headers
// Topic id : part4/section01/precompiled_headers
//
// Covers: target_precompile_headers

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // target_precompile_headers(tgt PRIVATE <vector> <string>)
    std::string pch = "vector";
    assert(!pch.empty());
}

void demo_intermediate() {
    // PCH speeds cold builds; can hurt incremental if headers churn
    assert(true);
}

void demo_expert() {
    // Prefer stable, widely included headers only
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/precompiled_headers", run>;

}  // namespace
