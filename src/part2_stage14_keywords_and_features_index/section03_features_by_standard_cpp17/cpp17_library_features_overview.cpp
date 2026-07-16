// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section03_features_by_standard_cpp17
// Item     : cpp17_library_features_overview
// Topic id : part2/stage14/section03/cpp17_library_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <any>
#include <array>
#include <charconv>
#include <filesystem>
#include <optional>
#include <string_view>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section03/cpp17_library_features_overview";

constexpr std::array<std::string_view, 10> features{
    "optional", "variant", "any", "string_view", "filesystem", "charconv", "execution policies", "pmr", "byte", "apply",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::optional<int> optional{7};
    const std::variant<int, std::string_view> variant{std::string_view{"ready"}};
    const std::any erased = 42;
    int parsed{};
    const std::string_view digits = "23";
    const auto result = std::from_chars(digits.data(), digits.data() + digits.size(), parsed);
    LEARN_EXPECT_EQ(checks, optional.value(), 7);
    LEARN_EXPECT_EQ(checks, std::get<std::string_view>(variant), std::string_view{"ready"});
    LEARN_EXPECT_EQ(checks, std::any_cast<int>(erased), 42);
    LEARN_EXPECT(checks, result.ec == std::errc{});
    LEARN_EXPECT_EQ(checks, std::filesystem::path{"a/b.txt"}.extension().string(), std::string{".txt"});
    LEARN_EXPECT_EQ(checks, features.size(), 10U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section03/cpp17_library_features_overview", run>;

}  // namespace
