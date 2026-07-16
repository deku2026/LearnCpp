// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : switch_statement
// Topic id : part2/stage08/section01/switch_statement
//

#include "learn/example_support.hpp"

#include <string_view>

namespace {

enum class TokenKind { identifier, number, punctuation };

[[nodiscard]] std::string_view describe(const TokenKind kind) noexcept {
    switch (kind) {
        case TokenKind::identifier:
            return "identifier";
        case TokenKind::number:
            return "number";
        case TokenKind::punctuation:
            return "punctuation";
        default:
            return "unknown";
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section01/switch_statement"};
    LEARN_EXPECT_EQ(checks, describe(TokenKind::identifier), std::string_view{"identifier"});
    LEARN_EXPECT_EQ(checks, describe(TokenKind::number), std::string_view{"number"});
    LEARN_EXPECT_EQ(checks, describe(TokenKind::punctuation), std::string_view{"punctuation"});
    LEARN_EXPECT_EQ(checks, describe(static_cast<TokenKind>(99)), std::string_view{"unknown"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/switch_statement", run>;

}  // namespace
