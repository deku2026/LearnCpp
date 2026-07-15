// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : std_move_static_cast_to_rvalue
// Topic id : part2/stage02/section06/std_move_static_cast_to_rvalue
//
// Covers: std::move as static_cast to rvalue ref; does not move by itself

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

void demo_basics() {
    int x = 10;
    using R = decltype(std::move(x));
    static_assert(std::is_same_v<R, int&&>);

    int&& rr = std::move(x);
    LEARN_CHECK(rr == 10);
    // x still holds 10; move only changes value category of the expression
    LEARN_CHECK(x == 10);
}

void demo_intermediate() {
    std::string a = "payload";
    std::string b = std::move(a);  // move constructor selected because of xvalue
    LEARN_CHECK(b == "payload");
    // a is valid but unspecified; safe operations:
    a.assign("new");
    LEARN_CHECK(a == "new");

    // Equivalent cast form
    std::string c = "xyz";
    std::string d = static_cast<std::string&&>(c);
    LEARN_CHECK(d == "xyz");
}

void demo_expert() {
    // move of const yields const T&& — often falls back to copy
    const std::string cs = "const";
    std::string copy = std::move(cs);  // copy ctor (no move from const)
    LEARN_CHECK(copy == "const");
    LEARN_CHECK(cs == "const");

    // Containers
    std::vector<int> v{1, 2, 3, 4};
    std::vector<int> w = std::move(v);
    LEARN_CHECK(w.size() == 4);
    LEARN_CHECK(w[0] == 1);
    // v valid unspecified; clear is safe
    v.clear();
    LEARN_CHECK(v.empty());

    // Naming a moved-from object is still an lvalue
    std::string s = "a";
    std::string&& r = std::move(s);
    // r is a named rvalue ref => lvalue expressions of type string
    std::string t = r;  // copy, not move
    LEARN_CHECK(t == "a");
    std::string u = std::move(r);  // move
    LEARN_CHECK(u == "a");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/std_move_static_cast_to_rvalue", run>;

}  // namespace
