// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_language_feature_macros
// Topic id : part2/stage14/section09/cpp_language_feature_macros
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section09/cpp_language_feature_macros";

struct MacroValue {
    std::string_view name;
    long long value;
};

constexpr std::array values{
    MacroValue{"__cpp_constexpr", __cpp_constexpr},
    MacroValue{"__cpp_consteval", __cpp_consteval},
    MacroValue{"__cpp_concepts", __cpp_concepts},
    MacroValue{"__cpp_if_consteval", __cpp_if_consteval},
#if defined(__cpp_explicit_this_parameter)
    MacroValue{"__cpp_explicit_this_parameter", __cpp_explicit_this_parameter},
#else
    MacroValue{"__cpp_explicit_this_parameter", 0},
#endif
#if defined(__cpp_multidimensional_subscript)
    MacroValue{"__cpp_multidimensional_subscript", __cpp_multidimensional_subscript},
#else
    MacroValue{"__cpp_multidimensional_subscript", 0},
#endif
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, values[0].value >= 201907L);
    LEARN_EXPECT(checks, values[1].value >= 201811L);
    LEARN_EXPECT(checks, values[2].value >= 201907L);
    LEARN_EXPECT_EQ(checks, values.size(), 6U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage14/section09/cpp_language_feature_macros", run>;

}  // namespace
