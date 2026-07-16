// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : semver_and_lockfiles
// Topic id : part4/section02/semver_and_lockfiles
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <compare>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section02/semver_and_lockfiles";

struct Version {
    int major;
    int minor;
    int patch;
    auto operator<=>(const Version&) const = default;
};

bool accepts_caret(Version requested, Version candidate) {
    return candidate >= requested && candidate.major == requested.major;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr Version requested{2, 3, 0};
    LEARN_EXPECT(checks, accepts_caret(requested, {2, 9, 1}));
    LEARN_EXPECT(checks, !accepts_caret(requested, {3, 0, 0}));
    constexpr Version locked{2, 4, 7};
    LEARN_EXPECT(checks, accepts_caret(requested, locked));
    // A range states compatibility intent; a lockfile snapshots the resolved graph and revisions.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section02/semver_and_lockfiles", run>;

}  // namespace
