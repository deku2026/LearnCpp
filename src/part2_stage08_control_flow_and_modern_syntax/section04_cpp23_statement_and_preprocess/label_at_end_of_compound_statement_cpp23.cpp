// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : label_at_end_of_compound_statement_cpp23
// Topic id : part2/stage08/section04/label_at_end_of_compound_statement_cpp23
//

#include "learn/example_support.hpp"

namespace {

[[nodiscard]] int labels_around_declarations() {
    int result = 0;
    {
        goto before_declaration;

    before_declaration:
        const int local = 42;  // C++23 permits a label immediately before a declaration.
        result = local;
        goto block_end;

    block_end:  // C++23 permits a label at the end of a compound statement.
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section04/label_at_end_of_compound_statement_cpp23"};
    LEARN_EXPECT_EQ(checks, labels_around_declarations(), 42);
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage08/section04/label_at_end_of_compound_statement_cpp23", run>;

}  // namespace
