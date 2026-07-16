// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_compat_cpp23
// Topic id : part4/section07/import_std_compat_cpp23
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section07/import_std_compat_cpp23";

struct NamespaceSurface {
    std::string_view facility;
    bool standard_namespace;
    bool global_compatibility_namespace;
};

constexpr std::array surfaces{
    NamespaceSurface{"C++ vector", true, false},
    NamespaceSurface{"C strlen compatibility", true, true},
    NamespaceSurface{"C atomic compatibility", true, true},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, surfaces[0].standard_namespace && !surfaces[0].global_compatibility_namespace);
    LEARN_EXPECT(checks, surfaces[1].global_compatibility_namespace);
    LEARN_EXPECT(checks, surfaces[2].global_compatibility_namespace);
    // A compatibility-oriented standard-library module has a wider surface than the standard namespace alone.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section07/import_std_compat_cpp23", run>;

}  // namespace
