// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : fallthrough_cpp17
// Topic id : part2/stage08/section03/fallthrough_cpp17
//

#include "learn/example_support.hpp"

namespace {

enum class Severity { informational, warning, critical };

[[nodiscard]] constexpr int escalation_score(const Severity severity) noexcept {
    int score = 0;
    switch (severity) {
        case Severity::critical:
            score += 100;
            [[fallthrough]];
        case Severity::warning:
            score += 10;
            [[fallthrough]];
        case Severity::informational:
            score += 1;
            break;
    }
    return score;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section03/fallthrough_cpp17"};
    static_assert(escalation_score(Severity::critical) == 111);
    LEARN_EXPECT_EQ(checks, escalation_score(Severity::informational), 1);
    LEARN_EXPECT_EQ(checks, escalation_score(Severity::warning), 11);
    LEARN_EXPECT_EQ(checks, escalation_score(Severity::critical), 111);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/fallthrough_cpp17", run>;

}  // namespace
