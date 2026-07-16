// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_target_basics
// Topic id : part4/section01/cmake_target_basics
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/cmake_target_basics";

enum class TargetKind { executable, static_library, shared_library, interface_library, object_library };

struct Target {
    std::string name;
    TargetKind kind;
    std::vector<std::string> sources;
};

bool valid(const Target& target) {
    if (target.name.empty()) return false;
    if (target.kind == TargetKind::interface_library) return target.sources.empty();
    return !target.sources.empty();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Target app{"learn_cpp", TargetKind::executable, {"main.cpp"}};
    const Target headers{"learn_headers", TargetKind::interface_library, {}};
    const Target broken{"", TargetKind::static_library, {"library.cpp"}};
    LEARN_EXPECT(checks, valid(app));
    LEARN_EXPECT(checks, valid(headers));
    LEARN_EXPECT(checks, !valid(broken));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/cmake_target_basics", run>;

}  // namespace
