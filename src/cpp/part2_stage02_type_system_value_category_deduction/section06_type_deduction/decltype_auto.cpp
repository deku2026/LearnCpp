// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : decltype_auto
// Topic id : part2/stage02/section06/decltype_auto
//
// Covers: decltype(auto) preserves ref/const from initializer expression

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

int g = 0;
int& get_ref() {
    return g;
}
int get_val() {
    return 42;
}

decltype(auto) forward_ref(int& x) {
    return (x);
}
decltype(auto) forward_val() {
    return get_val();
}

void demo_basics() {
    int x = 10;
    decltype(auto) a = x;    // int
    decltype(auto) b = (x);  // int&
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    b = 11;
    assert(x == 11);
}

void demo_intermediate() {
    decltype(auto) r = get_ref();
    static_assert(std::is_same_v<decltype(r), int&>);
    r = 5;
    assert(g == 5);

    decltype(auto) v = get_val();
    static_assert(std::is_same_v<decltype(v), int>);
    assert(v == 42);

    int n = 3;
    auto plain = forward_ref(n);  // if returned int&, auto drops to int...
    // forward_ref returns decltype(auto) from (x) => int&
    decltype(auto) kept = forward_ref(n);
    static_assert(std::is_same_v<decltype(kept), int&>);
    kept = 4;
    assert(n == 4);
    assert(plain == 3 || plain == 4);  // plain is a copy of the value at call time... wait

    // Actually: auto plain = forward_ref(n); deduces int from int& initializer (copy).
    // After kept=4, plain still holds old value if it was copy-initialized before.
    // Order: plain was initialized when n was 3, so plain==3.
    assert(plain == 3);
}

void demo_expert() {
    std::string s = "text";
    decltype(auto) r1 = s;
    decltype(auto) r2 = (s);
    decltype(auto) r3 = std::move(s);
    static_assert(std::is_same_v<decltype(r1), std::string>);
    static_assert(std::is_same_v<decltype(r2), std::string&>);
    static_assert(std::is_same_v<decltype(r3), std::string&&>);
    assert(r2 == "text");

    // Useful for perfect return type of wrappers
    auto call_val = []() -> decltype(auto) { return get_val(); };
    auto call_ref = []() -> decltype(auto) { return get_ref(); };
    static_assert(std::is_same_v<decltype(call_val()), int>);
    static_assert(std::is_same_v<decltype(call_ref()), int&>);
    assert(call_val() == 42);
    call_ref() = 8;
    assert(g == 8);

    assert(forward_val() == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/decltype_auto", run>;

}  // namespace
