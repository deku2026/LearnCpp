// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section08_deprecations_and_removals_cpp23
// Item     : undeprecation_and_semantic_changes
// Topic id : part2/stage14/section08/undeprecation_and_semantic_changes
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <cstdio>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section08/undeprecation_and_semantic_changes";

struct Change {
    std::string_view area;
    std::string_view effect;
};

constexpr std::array changes{
    Change{"C compatibility headers", "moved out of Annex D; no longer blanket-deprecated"},
    Change{"comma in subscript", "repurposed for multidimensional operator[]"},
    Change{"implicit move", "simpler move-eligible expression rules"},
    Change{"range-for temporaries", "broader lifetime extension in the initializer"},
};

struct Grid {
    int data[2][2]{{1, 2}, {3, 4}};
    int operator[](std::size_t row, std::size_t column) const { return data[row][column]; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Grid grid;
    LEARN_EXPECT_EQ(checks, (grid[1, 0]), 3);
    LEARN_EXPECT_EQ(checks, changes.size(), 4U);
    LEARN_EXPECT(checks, std::string_view{changes[0].effect}.contains("no longer"));
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section08/undeprecation_and_semantic_changes", run>;

}  // namespace
