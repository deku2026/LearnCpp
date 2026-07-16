// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : transparent_comparator_heterogeneous_lookup
// Topic id : part2/stage10/section02/transparent_comparator_heterogeneous_lookup
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <map>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/transparent_comparator_heterogeneous_lookup";

using Index = std::map<std::string, int, std::less<>>;
static_assert(requires(const Index& index, std::string_view key) { index.find(key); });

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Index index{{"alpha", 1}, {"beta", 2}};
    constexpr std::string_view key = "beta";
    const auto found = index.find(key);  // No temporary std::string is required by the interface.
    LEARN_EXPECT(checks, found != index.end());
    LEARN_EXPECT_EQ(checks, found->second, 2);
    LEARN_EXPECT(checks, index.find(std::string_view{"missing"}) == index.end());
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section02/transparent_comparator_heterogeneous_lookup", run>;

}  // namespace
