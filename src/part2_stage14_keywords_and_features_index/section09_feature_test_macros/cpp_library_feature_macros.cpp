// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_library_feature_macros
// Topic id : part2/stage14/section09/cpp_library_feature_macros
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section09/cpp_library_feature_macros";

struct MacroValue {
    std::string_view name;
    long long value;
};

constexpr std::array values{
#if defined(__cpp_lib_expected)
    MacroValue{"__cpp_lib_expected", __cpp_lib_expected},
#else
    MacroValue{"__cpp_lib_expected", 0},
#endif
#if defined(__cpp_lib_format)
    MacroValue{"__cpp_lib_format", __cpp_lib_format},
#else
    MacroValue{"__cpp_lib_format", 0},
#endif
#if defined(__cpp_lib_ranges)
    MacroValue{"__cpp_lib_ranges", __cpp_lib_ranges},
#else
    MacroValue{"__cpp_lib_ranges", 0},
#endif
#if defined(__cpp_lib_jthread)
    MacroValue{"__cpp_lib_jthread", __cpp_lib_jthread},
#else
    MacroValue{"__cpp_lib_jthread", 0},
#endif
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, values.size(), 4U);
    LEARN_EXPECT(checks, values[2].value >= 201911L);
    // A zero records "unavailable"; code must test the facility's specified minimum revision.
    LEARN_EXPECT(checks, values[0].value == 0 || values[0].value >= 202202L);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage14/section09/cpp_library_feature_macros", run>;

}  // namespace
