// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : optional_and_expected_versus_magic_value
// Topic id : part2/stage15/section04/optional_and_expected_versus_magic_value
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <optional>
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section04/optional_and_expected_versus_magic_value";

int legacy_find(bool found) {
    return found ? 0 : -1;
}
std::optional<std::size_t> find_index(bool found) {
    return found ? std::optional<std::size_t>{0} : std::nullopt;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, legacy_find(false), -1);  // Caller must remember the sentinel convention.
    LEARN_EXPECT(checks, !find_index(false));
    LEARN_EXPECT_EQ(checks, find_index(true).value(), 0U);
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    const std::expected<std::size_t, std::string> failure{std::unexpected{"not indexed"}};
    LEARN_EXPECT_EQ(checks, failure.error(), std::string{"not indexed"});
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage15/section04/optional_and_expected_versus_magic_value", run>;

}  // namespace
