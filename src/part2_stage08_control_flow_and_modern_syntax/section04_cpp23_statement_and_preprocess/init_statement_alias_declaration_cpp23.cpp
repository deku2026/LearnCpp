// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : init_statement_alias_declaration_cpp23
// Topic id : part2/stage08/section04/init_statement_alias_declaration_cpp23
//

#include "learn/example_support.hpp"

#include <array>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section04/init_statement_alias_declaration_cpp23"};

    if (using Value = int; Value value = 7) {
        LEARN_EXPECT_EQ(checks, value, 7);
    } else {
        LEARN_EXPECT(checks, false);
    }

    int switch_result = 0;
    switch (using Code = unsigned; Code{2U}) {
        case 2U:
            switch_result = 20;
            break;
        default:
            switch_result = -1;
            break;
    }

    int sum = 0;
    for (using Element = int; const Element value : std::array{1, 2, 3}) {
        sum += value;
    }

    LEARN_EXPECT_EQ(checks, switch_result, 20);
    LEARN_EXPECT_EQ(checks, sum, 6);
    // P2360 intentionally introduced no dedicated feature-test macro.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/init_statement_alias_declaration_cpp23", run>;

}  // namespace
