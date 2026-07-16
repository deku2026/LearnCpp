// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : suppression_rules
// Topic id : part2/stage04/section03/suppression_rules
// References: [class.copy.ctor], [class.copy.assign], [class.dtor]

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/suppression_rules";

struct UserDeclaredDestructor {
    std::string value;
    ~UserDeclaredDestructor() {}
};

struct UserDeclaredMove {
    std::string value;
    UserDeclaredMove() = default;
    UserDeclaredMove(UserDeclaredMove&&) noexcept = default;
    UserDeclaredMove& operator=(UserDeclaredMove&&) noexcept = default;
};

static_assert(std::is_copy_constructible_v<UserDeclaredDestructor>);
static_assert(!std::is_nothrow_move_constructible_v<UserDeclaredDestructor>);
static_assert(!std::is_copy_constructible_v<UserDeclaredMove>);
static_assert(std::is_nothrow_move_constructible_v<UserDeclaredMove>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    UserDeclaredDestructor first{"copy fallback"};
    UserDeclaredDestructor second{std::move(first)};  // Uses the copy constructor, not a move ctor.
    LEARN_EXPECT_EQ(checks, first.value, std::string{"copy fallback"});
    LEARN_EXPECT_EQ(checks, second.value, std::string{"copy fallback"});

    UserDeclaredMove movable;
    movable.value = "moved";
    UserDeclaredMove destination{std::move(movable)};
    LEARN_EXPECT_EQ(checks, destination.value, std::string{"moved"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/suppression_rules", run>;

}  // namespace
