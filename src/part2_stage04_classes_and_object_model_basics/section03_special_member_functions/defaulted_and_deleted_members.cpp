// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : defaulted_and_deleted_members
// Topic id : part2/stage04/section03/defaulted_and_deleted_members
// References: [dcl.fct.def.default], [dcl.fct.def.delete]

#include "learn/example_support.hpp"

#include <compare>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/defaulted_and_deleted_members";

struct Value {
    int number{};
    auto operator<=>(const Value&) const = default;
};

class MoveOnlyToken {
public:
    MoveOnlyToken() = default;
    MoveOnlyToken(const MoveOnlyToken&) = delete;
    MoveOnlyToken& operator=(const MoveOnlyToken&) = delete;
    MoveOnlyToken(MoveOnlyToken&&) = default;
    MoveOnlyToken& operator=(MoveOnlyToken&&) = default;
};

static_assert(std::is_copy_constructible_v<Value>);
static_assert(!std::is_copy_constructible_v<MoveOnlyToken>);
static_assert(std::is_move_constructible_v<MoveOnlyToken>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr Value left{3};
    constexpr Value right{5};
    static_assert(left < right);

    MoveOnlyToken first;
    MoveOnlyToken second{std::move(first)};
    (void)second;
    LEARN_EXPECT(checks, left != right);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/defaulted_and_deleted_members", run>;

}  // namespace
