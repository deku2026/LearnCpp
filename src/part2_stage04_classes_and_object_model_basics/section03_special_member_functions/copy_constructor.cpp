// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : copy_constructor
// Topic id : part2/stage04/section03/copy_constructor
// References: [class.copy.ctor]

#include "learn/example_support.hpp"

#include <string>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/copy_constructor";

struct Document {
    std::string title;
    std::vector<int> revisions;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Document original{"design", {1, 2}};
    Document copy{original};
    copy.title = "copy";
    copy.revisions.push_back(3);

    LEARN_EXPECT_EQ(checks, original.title, std::string{"design"});
    LEARN_EXPECT_EQ(checks, original.revisions.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, copy.revisions.size(), std::size_t{3});
    LEARN_EXPECT(checks, original.revisions.data() != copy.revisions.data());

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/copy_constructor", run>;

}  // namespace
