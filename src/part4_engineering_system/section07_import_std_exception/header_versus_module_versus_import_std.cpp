// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : header_versus_module_versus_import_std
// Topic id : part4/section07/header_versus_module_versus_import_std
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section07/header_versus_module_versus_import_std";

struct DeliveryMode {
    std::string_view name;
    bool needs_scanning;
    bool needs_prebuilt_library_artifact;
    bool portable_in_this_repository;
};

constexpr std::array modes{
    DeliveryMode{"standard headers", false, false, true},
    DeliveryMode{"project modules", true, false, false},
    DeliveryMode{"standard-library module", true, true, false},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, modes[0].portable_in_this_repository);
    LEARN_EXPECT(checks, modes[2].needs_scanning && modes[2].needs_prebuilt_library_artifact);
    LEARN_EXPECT_EQ(checks, modes.size(), 3U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section07/header_versus_module_versus_import_std", run>;

}  // namespace
