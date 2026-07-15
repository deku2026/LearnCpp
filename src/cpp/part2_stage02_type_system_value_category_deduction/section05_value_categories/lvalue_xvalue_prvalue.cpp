// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : lvalue_xvalue_prvalue
// Topic id : part2/stage02/section05/lvalue_xvalue_prvalue
//
// Covers: primary categories lvalue / xvalue / prvalue; identity vs move-from

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

int returns_prvalue() {
    return 42;
}

int global_obj = 1;
int& returns_lvalue_ref() {
    return global_obj;
}

void demo_basics() {
    int x = 10;  // x is an lvalue
    int y = x;   // read from lvalue
    LEARN_CHECK(y == 10);

    int z = 20;  // 20 is a prvalue used to initialize
    LEARN_CHECK(z == 20);

    LEARN_CHECK(returns_prvalue() == 42);
    LEARN_CHECK(returns_lvalue_ref() == 1);
}

void demo_intermediate() {
    int x = 5;
    int& lr = x;                       // lvalue ref binds lvalue
    int&& rr = static_cast<int&&>(x);  // xvalue via cast (same as move for int)
    LEARN_CHECK(lr == 5);
    LEARN_CHECK(rr == 5);

    std::string s = "abc";
    std::string moved = std::move(s);  // std::move produces xvalue
    LEARN_CHECK(moved == "abc");

    // Temporary materialization: prvalue string used as const lvalue ref
    const std::string& ref = std::string("tmp");
    LEARN_CHECK(ref == "tmp");
}

void demo_expert() {
    // decltype on names vs parenthesized expressions (category-sensitive)
    int x = 1;
    static_assert(std::is_same_v<decltype(x), int>);     // entity type
    static_assert(std::is_same_v<decltype((x)), int&>);  // lvalue expression

    static_assert(std::is_same_v<decltype(1), int>);               // prvalue
    static_assert(std::is_same_v<decltype(std::move(x)), int&&>);  // xvalue

    // Overload selection by category
    auto which = [](int&) { return 1; };
    auto which_r = [](int&&) { return 2; };
    int v = 0;
    LEARN_CHECK(which(v) == 1);
    LEARN_CHECK(which_r(1) == 2);
    LEARN_CHECK(which_r(std::move(v)) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/lvalue_xvalue_prvalue", run>;

}  // namespace
