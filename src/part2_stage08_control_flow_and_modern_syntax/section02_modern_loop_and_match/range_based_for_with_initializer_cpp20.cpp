// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : range_based_for_with_initializer_cpp20
// Topic id : part2/stage08/section02/range_based_for_with_initializer_cpp20
//

#include "learn/example_support.hpp"

#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section02/range_based_for_with_initializer_cpp20"};
    std::vector<int> squares;

    for (std::vector<int> owned{1, 2, 3, 4}; int& value : owned) {
        value *= value;
        squares.push_back(value);
    }

    LEARN_EXPECT_EQ(checks, squares, (std::vector{1, 4, 9, 16}));

#if 0
    // `owned` belongs to the for statement and is intentionally out of scope here.
    owned.push_back(25);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/range_based_for_with_initializer_cpp20", run>;

}  // namespace
