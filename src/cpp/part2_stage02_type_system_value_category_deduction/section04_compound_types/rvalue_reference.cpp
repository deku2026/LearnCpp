// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : rvalue_reference
// Topic id : part2/stage02/section04/rvalue_reference
//
// Covers: T&& binds rvalues, std::move, overload resolution lvalue/rvalue

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

int classify(int&) {
    return 1;
}
int classify(int&&) {
    return 2;
}

void demo_basics() {
    int&& rr = 42;  // binds temporary
    assert(rr == 42);
    rr = 100;
    assert(rr == 100);

    static_assert(std::is_rvalue_reference_v<int&&>);
}

void demo_intermediate() {
    int x = 10;
    assert(classify(x) == 1);             // lvalue
    assert(classify(20) == 2);            // prvalue
    assert(classify(std::move(x)) == 2);  // xvalue via move

    std::string src = "payload";
    std::string dst = std::move(src);
    assert(dst == "payload");
    // src is valid but unspecified; safe to assign/clear
    src.clear();
    assert(src.empty());
}

void demo_expert() {
    // Named rvalue references are lvalues when used as expressions.
    int&& rr = 5;
    assert(classify(rr) == 1);             // named => lvalue
    assert(classify(std::move(rr)) == 2);  // cast back to rvalue

    // Move construction of containers
    std::string a = "abcdefgh";
    std::string b(std::move(a));
    assert(b == "abcdefgh");

    // Forwarding reference shape (auto&&)
    auto&& uref1 = 1;  // int&&
    auto&& uref2 = b;  // std::string&
    static_assert(std::is_same_v<decltype(uref1), int&&>);
    static_assert(std::is_same_v<decltype(uref2), std::string&>);
    assert(uref1 == 1);
    assert(uref2 == "abcdefgh");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/rvalue_reference", run>;

}  // namespace
