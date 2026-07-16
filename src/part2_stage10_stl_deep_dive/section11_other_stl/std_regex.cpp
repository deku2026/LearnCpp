// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_regex
// Topic id : part2/stage10/section11/std_regex
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <regex>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/std_regex";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::regex assignment{R"(([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([0-9]+))"};
    const std::string text = "answer = 42";
    std::smatch match;
    LEARN_EXPECT(checks, std::regex_match(text, match, assignment));
    LEARN_EXPECT_EQ(checks, match[1].str(), std::string{"answer"});
    LEARN_EXPECT_EQ(checks, match[2].str(), std::string{"42"});
    const std::string replaced = std::regex_replace("x=1 y=2", std::regex{R"(=([0-9]+))"}, "[$1]");
    LEARN_EXPECT_EQ(checks, replaced, std::string{"x[1] y[2]"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/std_regex", run>;

}  // namespace
