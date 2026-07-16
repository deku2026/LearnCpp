// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : nodiscard_cpp17
// Topic id : part2/stage08/section03/nodiscard_cpp17
//

#include "learn/example_support.hpp"

namespace {

struct [[nodiscard]] ParseResult {
    int value;
    bool valid;
};

[[nodiscard]] constexpr ParseResult parse_digit(const char character) noexcept {
    if (character >= '0' && character <= '9') {
        return {character - '0', true};
    }
    return {0, false};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section03/nodiscard_cpp17"};
    const ParseResult good = parse_digit('7');
    const ParseResult bad = parse_digit('x');
    LEARN_EXPECT(checks, good.valid);
    LEARN_EXPECT_EQ(checks, good.value, 7);
    LEARN_EXPECT(checks, !bad.valid);

#if 0
    parse_digit('3');  // Compilers should diagnose an accidentally discarded result.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/nodiscard_cpp17", run>;

}  // namespace
