// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : conan_profile
// Topic id : part4/section02/conan_profile
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section02/conan_profile";

using Profile = std::map<std::string, std::string>;

bool complete_host_profile(const Profile& profile) {
    for (const auto key : {"os", "arch", "compiler", "compiler.version", "compiler.cppstd", "build_type"}) {
        if (!profile.contains(key)) return false;
    }
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Profile profile{
        {"os", "Windows"},           {"arch", "x86_64"},        {"compiler", "msvc"},
        {"compiler.version", "194"}, {"compiler.cppstd", "23"}, {"build_type", "Release"},
    };
    LEARN_EXPECT(checks, complete_host_profile(profile));
    auto incomplete = profile;
    incomplete.erase("compiler.cppstd");
    LEARN_EXPECT(checks, !complete_host_profile(incomplete));
    // Detected profiles are starting guesses; checked-in profiles stabilize CI and cross builds.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section02/conan_profile", run>;

}  // namespace
