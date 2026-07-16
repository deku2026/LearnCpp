// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : std_move_static_cast
// Topic id : part2/stage05/section01/std_move_static_cast
// References: [utility.move], [expr.static.cast]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/std_move_static_cast";

constexpr std::string_view category(const std::string&) noexcept {
    return "lvalue";
}
constexpr std::string_view category(std::string&&) noexcept {
    return "rvalue";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::string text = "still here";
    LEARN_EXPECT_EQ(checks, category(text), std::string_view{"lvalue"});
    LEARN_EXPECT_EQ(checks, category(std::move(text)), std::string_view{"rvalue"});
    LEARN_EXPECT_EQ(checks, text, std::string{"still here"});  // std::move itself moved no data.

    std::string consumed = std::move(text);
    LEARN_EXPECT_EQ(checks, consumed, std::string{"still here"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/std_move_static_cast", run>;

}  // namespace
