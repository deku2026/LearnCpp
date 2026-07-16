// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : for_loop_basics
// Topic id : part2/stage08/section01/for_loop_basics
//

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section01/for_loop_basics"};
    constexpr std::array values{1, 2, 3, 4, 5};

    int indexed_sum = 0;
    for (std::size_t index = 0; index < values.size(); ++index) {
        indexed_sum += values[index];
    }

    int odd_sum = 0;
    for (const int value : values) {
        if (value % 2 == 0) {
            continue;
        }
        odd_sum += value;
    }

    int prefix_sum = 0;
    for (const int value : values) {
        if (value > 3) {
            break;
        }
        prefix_sum += value;
    }

    int empty_iterations = 0;
    for ([[maybe_unused]] const int value : std::array<int, 0>{}) {
        ++empty_iterations;
    }

    LEARN_EXPECT_EQ(checks, indexed_sum, 15);
    LEARN_EXPECT_EQ(checks, odd_sum, 9);
    LEARN_EXPECT_EQ(checks, prefix_sum, 6);
    LEARN_EXPECT_EQ(checks, empty_iterations, 0);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/for_loop_basics", run>;

}  // namespace
