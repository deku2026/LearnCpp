// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : elifdef_elifndef_warning_recap
// Topic id : part2/stage08/section04/elifdef_elifndef_warning_recap
//

#include "learn/example_support.hpp"

#define LEARNCPP_STAGE8_PRIMARY

#if 0
inline constexpr int selected_branch = 0;
#elifdef LEARNCPP_STAGE8_PRIMARY
inline constexpr int selected_branch = 1;
#else
inline constexpr int selected_branch = -1;
#endif

#if 0
inline constexpr bool missing_branch_selected = false;
#elifndef LEARNCPP_STAGE8_NOT_DEFINED
inline constexpr bool missing_branch_selected = true;
#else
inline constexpr bool missing_branch_selected = false;
#endif

#if defined(LEARNCPP_ENABLE_WARNING_DEMO)
#warning "C++23 standardized #warning; this demo is opt-in so /WX builds stay clean"
#endif

#undef LEARNCPP_STAGE8_PRIMARY

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section04/elifdef_elifndef_warning_recap"};
    LEARN_EXPECT_EQ(checks, selected_branch, 1);
    LEARN_EXPECT(checks, missing_branch_selected);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/elifdef_elifndef_warning_recap", run>;

}  // namespace
