// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : goto_and_labels
// Topic id : part2/stage08/section01/goto_and_labels
//

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

struct Position {
    int row;
    int column;

    friend bool operator==(const Position&, const Position&) = default;
};

[[nodiscard]] Position locate(const int needle) {
    constexpr std::array<std::array<int, 3>, 2> grid{{{{2, 4, 6}}, {{8, 10, 12}}}};
    Position result{-1, -1};

    for (std::size_t row = 0; row < grid.size(); ++row) {
        for (std::size_t column = 0; column < grid[row].size(); ++column) {
            if (grid[row][column] == needle) {
                result = {static_cast<int>(row), static_cast<int>(column)};
                goto search_finished;
            }
        }
    }

search_finished:
    return result;
}

[[nodiscard]] int positive_or_zero(const int value) {
    int result = 0;
    if (value <= 0) {
        goto finished;
    }
    result = value;

finished:
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section01/goto_and_labels"};
    LEARN_EXPECT_EQ(checks, locate(10), (Position{1, 1}));
    LEARN_EXPECT_EQ(checks, locate(7), (Position{-1, -1}));
    LEARN_EXPECT_EQ(checks, positive_or_zero(9), 9);
    LEARN_EXPECT_EQ(checks, positive_or_zero(-9), 0);

#if 0
    // Ill-formed: a goto may not jump into the scope of an initialized object.
    goto inside;
    {
        int initialized = 42;
    inside:
        (void)initialized;
    }
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/goto_and_labels", run>;

}  // namespace
