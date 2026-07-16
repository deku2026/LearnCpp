// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : if_statement
// Topic id : part2/stage08/section01/if_statement
//

#include "learn/example_support.hpp"

#include <string_view>

namespace {

[[nodiscard]] std::string_view classify(const int value) noexcept {
    if (value < 0) {
        return "negative";
    } else if (value == 0) {
        return "zero";
    }
    return "positive";
}

[[nodiscard]] bool record_call(bool& called) noexcept {
    called = true;
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section01/if_statement"};
    LEARN_EXPECT_EQ(checks, classify(-3), std::string_view{"negative"});
    LEARN_EXPECT_EQ(checks, classify(0), std::string_view{"zero"});
    LEARN_EXPECT_EQ(checks, classify(3), std::string_view{"positive"});

    bool right_hand_side_called = false;
    if (false && record_call(right_hand_side_called)) {
        LEARN_EXPECT(checks, false);
    }
    LEARN_EXPECT(checks, !right_hand_side_called);

    int branch = 0;
    if (true || record_call(right_hand_side_called)) {
        branch = 1;
    }
    LEARN_EXPECT_EQ(checks, branch, 1);
    LEARN_EXPECT(checks, !right_hand_side_called);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/if_statement", run>;

}  // namespace
