// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : public_private_interface
// Topic id : part4/section01/public_private_interface
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/public_private_interface";

enum class Scope { private_, public_, interface_ };

struct UsageRequirements {
    std::set<std::string> building_target;
    std::set<std::string> consuming_target;
};

void add(UsageRequirements& result, Scope scope, std::string property) {
    if (scope != Scope::interface_) result.building_target.insert(property);
    if (scope != Scope::private_) result.consuming_target.insert(std::move(property));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    UsageRequirements requirements;
    add(requirements, Scope::private_, "implementation-only");
    add(requirements, Scope::public_, "needed-by-both");
    add(requirements, Scope::interface_, "consumer-only");
    LEARN_EXPECT_EQ(checks, requirements.building_target.size(), 2U);
    LEARN_EXPECT_EQ(checks, requirements.consuming_target.size(), 2U);
    LEARN_EXPECT(checks, !requirements.consuming_target.contains("implementation-only"));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/public_private_interface", run>;

}  // namespace
