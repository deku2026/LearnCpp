// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : backslash_continuation_trailing_whitespace_cpp23
// Topic id : part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23
//

#include "learn/example_support.hpp"

// clang-format off
#define LEARNCPP_STAGE8_TOTAL(first, second, third) \
    ((first) + (second) + (third))
// clang-format on

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23"};
    // clang-format off
    constexpr int continued_expression = 10 + \
                                         20;
    // clang-format on
    constexpr int macro_result = LEARNCPP_STAGE8_TOTAL(1, 2, 3);

    LEARN_EXPECT_EQ(checks, continued_expression, 30);
    LEARN_EXPECT_EQ(checks, macro_result, 6);

    // C++23 also permits whitespace between a backslash and a physical newline.
    // This file keeps the backslash last so editors and whitespace checks cannot
    // silently alter the lesson while still exercising the line-splicing phase.
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23", run>;

}  // namespace

#undef LEARNCPP_STAGE8_TOTAL
