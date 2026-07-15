// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : auto_decay_copy_cpp23
// Topic id : part2/stage02/section06/auto_decay_copy_cpp23
//
// Covers: C++23 auto(x)/auto{x} decay-copy; force prvalue materialization

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <version>

namespace {

void demo_basics() {
    int x = 10;
    const int& rx = x;

    // Classic copy via auto variable
    auto copy = rx;
    static_assert(std::is_same_v<decltype(copy), int>);
    LEARN_CHECK(copy == 10);

    // C++23 decay-copy expression when available
#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    auto c2 = auto(rx);
    static_assert(std::is_same_v<decltype(c2), int>);
    LEARN_CHECK(c2 == 10);
#else
    auto c2 = static_cast<std::decay_t<decltype(rx)>>(rx);
    LEARN_CHECK(c2 == 10);
#endif
}

void demo_intermediate() {
    std::string s = "hello";
    std::string& rs = s;

#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    auto pr = auto(rs);  // decay-copy to std::string prvalue then materialize
    static_assert(std::is_same_v<decltype(pr), std::string>);
    LEARN_CHECK(pr == "hello");
    pr += "!";
    LEARN_CHECK(s == "hello");  // original unchanged
#else
    auto pr = std::string(rs);
    LEARN_CHECK(pr == "hello");
    pr += "!";
    LEARN_CHECK(s == "hello");
#endif

    // Useful to pass a copy into APIs expecting values without naming a temporary.
    auto takes_value = [](std::string v) { return v.size(); };
#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    LEARN_CHECK(takes_value(auto(s)) == 5);
#else
    LEARN_CHECK(takes_value(std::string(s)) == 5);
#endif
}

void demo_expert() {
    int arr[3] = {1, 2, 3};
    // auto(arr) would decay to pointer in C++23 auto cast
#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    auto p = auto(arr);
    static_assert(std::is_same_v<decltype(p), int*>);
    LEARN_CHECK(p[1] == 2);
#else
    auto p = +arr;  // force decay to pointer
    static_assert(std::is_same_v<decltype(p), int*>);
    LEARN_CHECK(p[1] == 2);
#endif

    const int n = 3;
#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    auto m = auto{n};
    static_assert(std::is_same_v<decltype(m), int>);
    LEARN_CHECK(m == 3);
#else
    auto m = int{n};
    LEARN_CHECK(m == 3);
#endif

    // Contrast with decltype(auto) which would keep reference
    int x = 1;
    int& r = x;
    decltype(auto) kept = r;
    static_assert(std::is_same_v<decltype(kept), int&>);
    LEARN_CHECK(&kept == &x);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/auto_decay_copy_cpp23", run>;

}  // namespace
