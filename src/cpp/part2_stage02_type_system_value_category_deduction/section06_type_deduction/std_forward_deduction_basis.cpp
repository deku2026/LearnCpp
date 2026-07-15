// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : std_forward_deduction_basis
// Topic id : part2/stage02/section06/std_forward_deduction_basis
//
// Covers: perfect forwarding with T&& + std::forward; preserve value category

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

int take(std::string&) {
    return 1;
}
int take(const std::string&) {
    return 2;
}
int take(std::string&&) {
    return 3;
}

template <class T>
int forward_to_take(T&& arg) {
    return take(std::forward<T>(arg));
}

template <class T>
decltype(auto) forward_identity(T&& arg) {
    return std::forward<T>(arg);
}

void demo_basics() {
    std::string s = "x";
    const std::string cs = "y";

    assert(forward_to_take(s) == 1);
    assert(forward_to_take(cs) == 2);
    assert(forward_to_take(std::string{"z"}) == 3);
    assert(forward_to_take(std::move(s)) == 3);
}

void demo_intermediate() {
    int n = 5;
    decltype(auto) a = forward_identity(n);
    decltype(auto) b = forward_identity(5);
    static_assert(std::is_same_v<decltype(a), int&>);
    static_assert(std::is_same_v<decltype(b), int&&>);
    assert(a == 5);
    assert(b == 5);

    // Without forward, named parameter is always an lvalue (even if T deduced as string).
    auto broken = [](auto&& arg) { return take(arg); };
    // prvalue string binds as string&& param type, but name `arg` is lvalue => take(string&)
    assert(broken(std::string{"t"}) == 1);
}

void demo_expert() {
    // Multi-hop factory
    auto emplace_like = [](auto&& x) { return forward_to_take(std::forward<decltype(x)>(x)); };
    std::string local = "L";
    assert(emplace_like(local) == 1);
    assert(emplace_like(std::move(local)) == 3);

    // Type relationship of forward
    using T = int&;
    int v = 1;
    static_assert(std::is_same_v<decltype(std::forward<T>(v)), int&>);

    using U = int;
    static_assert(std::is_same_v<decltype(std::forward<U>(1)), int&&>);

    // Forwarding into constructor (template at namespace scope below via free function)
    std::string src = "box";
    std::string b1 = std::forward<std::string&>(src);
    assert(b1 == "box");
    assert(src == "box");
    std::string b2 = std::forward<std::string>(std::move(src));
    assert(b2 == "box");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/std_forward_deduction_basis", run>;

}  // namespace
