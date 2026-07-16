// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : ninja_backend
// Topic id : part4/section01/ninja_backend
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

constexpr std::string_view kTopic = "part4/section01/ninja_backend";

bool build(std::string_view node, const std::map<std::string, std::vector<std::string>>& graph,
           std::set<std::string>& active, std::vector<std::string>& order) {
    if (std::ranges::find(order, node) != order.end()) return true;
    if (!active.insert(std::string{node}).second) return false;
    for (const auto& dependency : graph.at(std::string{node})) {
        if (!build(dependency, graph, active, order)) return false;
    }
    active.erase(std::string{node});
    order.emplace_back(node);
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::map<std::string, std::vector<std::string>> graph{
        {"compile-a", {}},
        {"compile-b", {}},
        {"link", {"compile-a", "compile-b"}},
    };
    std::set<std::string> active;
    std::vector<std::string> order;
    LEARN_EXPECT(checks, build("link", graph, active, order));
    LEARN_EXPECT_EQ(checks, order.back(), std::string{"link"});
    LEARN_EXPECT_EQ(checks, order.size(), 3U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/ninja_backend", run>;

}  // namespace
