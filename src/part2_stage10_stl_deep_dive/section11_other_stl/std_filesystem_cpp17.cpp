// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_filesystem_cpp17
// Topic id : part2/stage10/section11/std_filesystem_cpp17
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <filesystem>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/std_filesystem_cpp17";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::filesystem::path input{"assets/../assets/config.json"};
    const auto normalized = input.lexically_normal();
    LEARN_EXPECT_EQ(checks, normalized.filename().string(), std::string{"config.json"});
    LEARN_EXPECT_EQ(checks, normalized.parent_path().filename().string(), std::string{"assets"});
    LEARN_EXPECT_EQ(checks, normalized.extension().string(), std::string{".json"});
    const auto relative = normalized.lexically_relative("assets");
    LEARN_EXPECT_EQ(checks, relative.generic_string(), std::string{"config.json"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/std_filesystem_cpp17", run>;

}  // namespace
