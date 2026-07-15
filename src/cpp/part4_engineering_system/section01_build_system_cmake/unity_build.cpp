// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : unity_build
// Topic id : part4/section01/unity_build
//
// Covers: CMAKE_UNITY_BUILD batch compilation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // set(CMAKE_UNITY_BUILD ON) or target property UNITY_BUILD
    const bool unity = true;
    assert(unity);
}

void demo_intermediate() {
    // Watch for anonymous namespace / static symbol collisions across TU batches
    assert(true);
}

void demo_expert() {
    // Exclude problematic files via SKIP_UNITY_BUILD_INCLUSION
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/unity_build", run>;

}  // namespace
