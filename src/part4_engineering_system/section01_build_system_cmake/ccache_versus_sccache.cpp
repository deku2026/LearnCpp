// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : ccache_versus_sccache
// Topic id : part4/section01/ccache_versus_sccache
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/ccache_versus_sccache";

struct CacheTool {
    std::string_view name;
    bool local_cache;
    bool remote_cache;
    bool distributed_compilation;
};

constexpr std::array tools{
    CacheTool{"ccache", true, true, false},
    CacheTool{"sccache", true, true, true},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, tools[0].local_cache && tools[1].local_cache);
    LEARN_EXPECT(checks, tools[0].remote_cache && tools[1].remote_cache);
    LEARN_EXPECT(checks, !tools[0].distributed_compilation && tools[1].distributed_compilation);
    // Cache keys must include compiler, flags, environment and source dependencies for correctness.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/ccache_versus_sccache", run>;

}  // namespace
