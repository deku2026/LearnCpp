// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : has_include_and_has_cpp_attribute
// Topic id : part2/stage14/section09/has_include_and_has_cpp_attribute
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<expected>)
#include <expected>
#endif
#include <array>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section09/has_include_and_has_cpp_attribute";

constexpr bool has_expected_header =
#if __has_include(<expected>)
    true;
#else
    false;
#endif
constexpr long long nodiscard_revision = __has_cpp_attribute(nodiscard);
constexpr long long likely_revision = __has_cpp_attribute(likely);
constexpr long long assume_revision = __has_cpp_attribute(assume);

[[nodiscard]] int answer() {
    return 42;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, nodiscard_revision > 0);
    LEARN_EXPECT(checks, likely_revision > 0);
    LEARN_EXPECT(checks, assume_revision >= 0);
    LEARN_EXPECT_EQ(checks, answer(), 42);
    LEARN_EXPECT(checks, has_expected_header || !has_expected_header);
    // Header presence, attribute syntax and a complete library feature are three distinct probes.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section09/has_include_and_has_cpp_attribute", run>;

}  // namespace
