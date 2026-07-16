// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_split
// Topic id : part2/stage10/section07/pipe_split
//

#include "learn/example_support.hpp"

#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_split"};
    std::string text{"alpha,,beta"};
    auto parts = text | std::views::split(',');

    text.front() = 'A';  // The split view refers to the source; no tokens exist yet.
    std::vector<std::string> tokens;
    for (const auto part : parts) {
        tokens.emplace_back(part.begin(), part.end());
    }

    LEARN_EXPECT_EQ(checks, tokens, (std::vector<std::string>{"Alpha", "", "beta"}));

    constexpr std::string_view path{"usr--local--bin"};
    constexpr std::string_view separator{"--"};
    std::vector<std::string> components;
    for (const auto component : path | std::views::split(separator)) {
        components.emplace_back(component.begin(), component.end());
    }
    LEARN_EXPECT_EQ(checks, components, (std::vector<std::string>{"usr", "local", "bin"}));

#if 0
    auto dangling = [] {
        std::string local{"a,b"};
        return std::string_view{local} | std::views::split(',');
    }();  // The returned view refers to destroyed string storage.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_split", run>;

}  // namespace
