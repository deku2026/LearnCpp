// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : ref_qualified_member_functions
// Topic id : part2/stage04/section06/ref_qualified_member_functions
// References: [dcl.fct], [class.mfct.non.static]

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section06/ref_qualified_member_functions";

class Text {
public:
    explicit Text(std::string value) : value_(std::move(value)) {}

    [[nodiscard]] const std::string& value() const& noexcept { return value_; }
    [[nodiscard]] std::string value() && noexcept { return std::move(value_); }

private:
    std::string value_;
};

static_assert(std::is_same_v<decltype(std::declval<const Text&>().value()), const std::string&>);
static_assert(std::is_same_v<decltype(std::declval<Text&&>().value()), std::string>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Text stable{"borrowed"};
    const std::string& borrowed = stable.value();
    std::string owned = Text{"temporary"}.value();

    LEARN_EXPECT_EQ(checks, borrowed, std::string{"borrowed"});
    LEARN_EXPECT_EQ(checks, owned, std::string{"temporary"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section06/ref_qualified_member_functions", run>;

}  // namespace
