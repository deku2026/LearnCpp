// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : structured_bindings_cpp17
// Topic id : part2/stage08/section02/structured_bindings_cpp17
//

#include "learn/example_support.hpp"

#include <map>
#include <string>
#include <type_traits>

namespace {

struct Record {
    int id;
    std::string name;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section02/structured_bindings_cpp17"};
    Record original{7, "Ada"};

    auto [copied_id, copied_name] = original;
    copied_id = 8;
    copied_name = "copy";
    LEARN_EXPECT_EQ(checks, copied_id, 8);
    LEARN_EXPECT_EQ(checks, copied_name, std::string{"copy"});
    LEARN_EXPECT_EQ(checks, original.id, 7);
    LEARN_EXPECT_EQ(checks, original.name, std::string{"Ada"});

    auto& [id, name] = original;
    static_assert(std::is_same_v<decltype(id), int>);
    static_assert(std::is_same_v<decltype((id)), int&>);
    id = 9;
    name = "Grace";
    LEARN_EXPECT_EQ(checks, original.id, 9);
    LEARN_EXPECT_EQ(checks, original.name, std::string{"Grace"});

    const std::map<std::string, int> scores{{"Ada", 10}, {"Grace", 20}};
    int total = 0;
    for (const auto& [person, score] : scores) {
        LEARN_EXPECT(checks, !person.empty());
        total += score;
    }
    LEARN_EXPECT_EQ(checks, total, 30);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/structured_bindings_cpp17", run>;

}  // namespace
