// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : rule_of_zero
// Topic id : part2/stage04/section03/rule_of_zero
// References: [class.copy.ctor], C++ Core Guidelines C.20

#include "learn/example_support.hpp"

#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/rule_of_zero";

struct Project {
    std::string name;
    std::vector<int> milestones;
    std::optional<std::string> owner;
};

static_assert(std::is_copy_constructible_v<Project>);
static_assert(std::is_move_constructible_v<Project>);
static_assert(std::is_destructible_v<Project>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Project original{"compiler", {1, 2}, "team"};
    Project copy = original;
    copy.milestones.push_back(3);
    Project moved = std::move(copy);

    LEARN_EXPECT_EQ(checks, original.milestones.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, moved.milestones.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, moved.owner.value(), std::string{"team"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/rule_of_zero", run>;

}  // namespace
