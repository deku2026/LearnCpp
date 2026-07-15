// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : multi_config
// Topic id : part4/section01/multi_config
//
// Covers: single-config vs multi-config generators

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Ninja/Makefiles: CMAKE_BUILD_TYPE; VS/Xcode: --config Debug
    std::string single = "Debug";
    assert(single == "Debug");
}

void demo_intermediate() {
    // Multi-config builds all configs in one build tree
    const char* configs[] = {"Debug", "Release", "RelWithDebInfo"};
    assert(std::string_view{configs[0]} == "Debug");
}

void demo_expert() {
    // Prefer generator expressions over branching on CMAKE_BUILD_TYPE alone
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/multi_config", run>;

}  // namespace
