// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : semver_and_lockfiles
// Topic id : part4/section02/semver_and_lockfiles
//
// Covers: semver ranges and lockfiles for reproducible builds

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // MAJOR.MINOR.PATCH — breaking/feature/fix
    int major = 1, minor = 2, patch = 3;
    LEARN_CHECK(major == 1 && minor == 2 && patch == 3);
}

void demo_intermediate() {
    // Lockfiles pin exact resolved versions
    LEARN_CHECK(true);
}

void demo_expert() {
    // CI should fail on undelcared dependency drift
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/semver_and_lockfiles", run>;

}  // namespace
