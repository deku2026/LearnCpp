// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section06_features_by_standard_cpp23_library
// Item     : cpp23_library_feature_checklist
// Topic id : part2/stage14/section06/cpp23_library_feature_checklist
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <string_view>
#if __has_include(<expected>)
#include <expected>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section06/cpp23_library_feature_checklist";

struct Feature {
    std::string_view name;
    bool available;
};

constexpr std::array features{
    Feature{"expected",
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
            true
#else
            false
#endif
    },
    Feature{"print",
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
            true
#else
            false
#endif
    },
    Feature{"mdspan",
#if defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
            true
#else
            false
#endif
    },
    Feature{"generator",
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
            true
#else
            false
#endif
    },
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, features.size(), 4U);
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    const std::expected<int, int> value{23};
    LEARN_EXPECT_EQ(checks, value.value(), 23);
#else
    LEARN_EXPECT(checks, !features[0].available);
#endif
    // Availability is per facility and library; a C++23 language mode alone is insufficient.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section06/cpp23_library_feature_checklist", run>;

}  // namespace
