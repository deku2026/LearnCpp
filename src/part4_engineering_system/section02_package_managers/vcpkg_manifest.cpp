// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : vcpkg_manifest
// Topic id : part4/section02/vcpkg_manifest
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section02/vcpkg_manifest";

struct Manifest {
    std::string name;
    std::string builtin_baseline;
    std::vector<std::string> dependencies;
};

bool reproducible_enough(const Manifest& manifest) {
    return !manifest.name.empty() && manifest.builtin_baseline.size() == 40 && !manifest.dependencies.empty();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Manifest pinned{"learn-app", std::string(40, 'a'), {"fmt", "zlib[tools]"}};
    const Manifest floating{"learn-app", {}, {"fmt"}};
    LEARN_EXPECT(checks, reproducible_enough(pinned));
    LEARN_EXPECT(checks, !reproducible_enough(floating));
    LEARN_EXPECT_EQ(checks, pinned.dependencies.size(), 2U);
    // A baseline selects a registry version set; per-port minimums and overrides answer different needs.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section02/vcpkg_manifest", run>;

}  // namespace
