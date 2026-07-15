// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : glvalue_and_rvalue
// Topic id : part2/stage02/section05/glvalue_and_rvalue
//
// Covers: glvalue = lvalue|xvalue; rvalue = xvalue|prvalue; taxonomy diagram

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// Taxonomy:
//   expression
//   ├── glvalue  (has identity)
//   │   ├── lvalue
//   │   └── xvalue
//   └── rvalue   (can be moved from / pure value)
//       ├── xvalue
//       └── prvalue

int sink_rvalue(std::string&&) {
    return 1;
}
int sink_glvalue_like(const std::string&) {
    return 2;
}

void demo_basics() {
    int x = 1;      // x: lvalue, hence glvalue
    int y = x + 2;  // x+2: prvalue, hence rvalue
    assert(y == 3);

    int&& xr = std::move(x);  // std::move(x): xvalue (both glvalue and rvalue)
    assert(xr == 1);
}

void demo_intermediate() {
    std::string s = "data";

    // const lvalue ref can bind all glvalues and also materialize prvalues
    assert(sink_glvalue_like(s) == 2);
    assert(sink_glvalue_like(std::string("pr")) == 2);

    // rvalue ref prefers rvalues (xvalue/prvalue)
    assert(sink_rvalue(std::move(s)) == 1);
    assert(sink_rvalue(std::string("tmp")) == 1);

    // After move, s is valid but unspecified
    s = "reset";
    assert(s == "reset");
}

void demo_expert() {
    int a = 10;
    // Properties checked via decltype of expressions
    static_assert(std::is_lvalue_reference_v<decltype((a))>);           // lvalue
    static_assert(std::is_rvalue_reference_v<decltype(std::move(a))>);  // xvalue as int&&
    static_assert(!std::is_reference_v<decltype(a + 1)>);               // prvalue int

    // Address-of requires glvalue with identity (lvalue or xvalue of object)
    int* p = &a;
    assert(p == &a);

    // Comma and conditional preserve categories carefully
    int b = 2;
    int& chosen = (a > b) ? a : b;  // result is lvalue
    chosen = 99;
    assert(a == 99 || b == 99);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/glvalue_and_rvalue", run>;

}  // namespace
