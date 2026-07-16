// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : target_link_include_directories
// Topic id : part4/section01/target_link_include_directories
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/target_link_include_directories";

struct Target {
    std::set<std::string> private_includes;
    std::set<std::string> interface_includes;
    std::set<std::string> links;
};

std::set<std::string> compile_includes(const Target& target, const std::map<std::string, Target>& graph) {
    auto result = target.private_includes;
    result.insert(target.interface_includes.begin(), target.interface_includes.end());
    for (const auto& dependency : target.links) {
        const auto& exported = graph.at(dependency).interface_includes;
        result.insert(exported.begin(), exported.end());
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::map<std::string, Target> graph{
        {"core", {{"core/src"}, {"core/include"}, {}}},
        {"app", {{"app/src"}, {}, {"core"}}},
    };
    const auto includes = compile_includes(graph.at("app"), graph);
    LEARN_EXPECT(checks, includes.contains("app/src"));
    LEARN_EXPECT(checks, includes.contains("core/include"));
    LEARN_EXPECT(checks, !includes.contains("core/src"));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/target_link_include_directories", run>;

}  // namespace
