// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : install_export_find_package
// Topic id : part4/section01/install_export_find_package
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/install_export_find_package";

struct Package {
    std::string version;
    std::map<std::string, std::string> imported_targets;
};

bool compatible(std::string_view requested, const Package& package) {
    return package.version.starts_with(requested);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Package package{"2.4.1", {{"Learn::Core", "lib/learn_core"}}};
    LEARN_EXPECT(checks, compatible("2.", package));
    LEARN_EXPECT(checks, !compatible("3.", package));
    LEARN_EXPECT_EQ(checks, package.imported_targets.at("Learn::Core"), std::string{"lib/learn_core"});
    // Installation exports targets plus usage requirements; consumers should not reconstruct raw paths.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/install_export_find_package", run>;

}  // namespace
