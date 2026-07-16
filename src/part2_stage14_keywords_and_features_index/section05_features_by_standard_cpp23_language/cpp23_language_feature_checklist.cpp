// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section05_features_by_standard_cpp23_language
// Item     : cpp23_language_feature_checklist
// Topic id : part2/stage14/section05/cpp23_language_feature_checklist
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section05/cpp23_language_feature_checklist";

constexpr int choose(bool constant_path) {
    if consteval {
        return 23;
    } else {
        return constant_path ? 24 : 25;
    }
}

struct Matrix {
    int values[2][2]{{1, 2}, {3, 4}};
    int& operator[](std::size_t row, std::size_t column) { return values[row][column]; }
};

constexpr std::array<std::string_view, 11> features{
    "if consteval",
    "explicit object parameter",
    "multidimensional subscript",
    "static operator() / []",
    "auto(x)",
    "labels at end",
    "#elifdef/#elifndef",
    "size_t literals",
    "extended floating types",
    "simpler implicit move",
    "attributes on lambdas",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr int compile_time = choose(true);
    Matrix matrix;
    matrix[1, 0] = 8;
    auto copied = auto(matrix.values[0][0]);
    static_assert(compile_time == 23);
    LEARN_EXPECT_EQ(checks, choose(false), 25);
    LEARN_EXPECT_EQ(checks, (matrix[1, 0]), 8);
    LEARN_EXPECT_EQ(checks, copied, 1);
    LEARN_EXPECT_EQ(checks, features.size(), 11U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section05/cpp23_language_feature_checklist", run>;

}  // namespace
