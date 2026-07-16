// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : heterogeneous_erase_extract_cpp23
// Topic id : part2/stage10/section02/heterogeneous_erase_extract_cpp23
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <map>
#include <string>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/heterogeneous_erase_extract_cpp23";

using Index = std::map<std::string, int, std::less<>>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Index index{{"alpha", 1}, {"beta", 2}};
#if defined(__cpp_lib_associative_heterogeneous_erasure) && __cpp_lib_associative_heterogeneous_erasure >= 202110L
    LEARN_EXPECT_EQ(checks, index.erase(std::string_view{"alpha"}), 1U);
    auto node = index.extract(std::string_view{"beta"});
#else
    auto alpha = index.find(std::string_view{"alpha"});
    LEARN_EXPECT(checks, alpha != index.end());
    index.erase(alpha);
    auto node = index.extract(index.find(std::string_view{"beta"}));
#endif
    LEARN_EXPECT(checks, !node.empty());
    node.key() = "gamma";
    index.insert(std::move(node));
    LEARN_EXPECT(checks, index.contains("gamma"));
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section02/heterogeneous_erase_extract_cpp23", run>;

}  // namespace
