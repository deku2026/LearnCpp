// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : most_vexing_parse_and_brace_fix
// Topic id : part2/stage02/section02/most_vexing_parse_and_brace_fix
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Token {
    int value{};
};

using WhatTokenWithParensWouldDeclare = Token();

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_function_v<WhatTokenWithParensWouldDeclare>);
    Token object{};  // Braces unambiguously define an object.
    Token initialized{42};

    learn::ExampleChecks checks{"part2/stage02/section02/most_vexing_parse_and_brace_fix"};
    LEARN_EXPECT_EQ(checks, object.value, 0);
    LEARN_EXPECT_EQ(checks, initialized.value, 42);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/most_vexing_parse_and_brace_fix", run>;

}  // namespace
