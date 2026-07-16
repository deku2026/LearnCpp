// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : reference_collapsing_rules
// Topic id : part2/stage05/section02/reference_collapsing_rules
// References: [dcl.ref]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section02/reference_collapsing_rules";

template <class T>
using LvalueOf = T&;

template <class T>
using RvalueOf = T&&;

static_assert(std::is_same_v<LvalueOf<int&>, int&>);    // &  + &  -> &
static_assert(std::is_same_v<LvalueOf<int&&>, int&>);   // && + &  -> &
static_assert(std::is_same_v<RvalueOf<int&>, int&>);    // &  + && -> &
static_assert(std::is_same_v<RvalueOf<int&&>, int&&>);  // && + && -> &&

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int value = 3;
    RvalueOf<int&> collapsed = value;
    collapsed = 7;
    LEARN_EXPECT_EQ(checks, value, 7);
    LEARN_EXPECT(checks, (std::is_lvalue_reference_v<decltype(collapsed)>));

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section02/reference_collapsing_rules", run>;

}  // namespace
