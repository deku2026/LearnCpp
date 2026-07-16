// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : if_switch_with_initializer_cpp17
// Topic id : part2/stage08/section02/if_switch_with_initializer_cpp17
//

#include "learn/example_support.hpp"

#include <map>
#include <string>
#include <string_view>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section02/if_switch_with_initializer_cpp17"};
    std::map<std::string, int> scores{{"Ada", 90}};

    if (const auto [position, inserted] = scores.try_emplace("Grace", 95); inserted) {
        LEARN_EXPECT_EQ(checks, position->second, 95);
    } else {
        LEARN_EXPECT(checks, false);
    }

    if (const auto [position, inserted] = scores.try_emplace("Ada", 100); !inserted) {
        LEARN_EXPECT_EQ(checks, position->second, 90);
    } else {
        LEARN_EXPECT(checks, false);
    }

    std::string_view category;
    switch (const int remainder = 14 % 3; remainder) {
        case 0:
            category = "divisible";
            break;
        case 1:
            category = "remainder one";
            break;
        default:
            category = "remainder two";
            break;
    }

    LEARN_EXPECT_EQ(checks, scores.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, category, std::string_view{"remainder two"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/if_switch_with_initializer_cpp17", run>;

}  // namespace
