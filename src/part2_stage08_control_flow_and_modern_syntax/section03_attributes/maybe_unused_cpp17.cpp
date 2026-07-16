// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : maybe_unused_cpp17
// Topic id : part2/stage08/section03/maybe_unused_cpp17
//

#include "learn/example_support.hpp"

#include <string_view>

namespace {

[[nodiscard]] int fixed_record_count([[maybe_unused]] const std::string_view diagnostic_tag) {
    [[maybe_unused]] constexpr int debug_only_schema_version = 3;
    return 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section03/maybe_unused_cpp17"};
    LEARN_EXPECT_EQ(checks, fixed_record_count("stage8-demo"), 2);

    [[maybe_unused]] const int value_used_only_in_debug_builds = 42;
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/maybe_unused_cpp17", run>;

}  // namespace
