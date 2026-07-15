// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : using_alias_replaces_typedef
// Topic id : part2/stage02/section07/using_alias_replaces_typedef
//
// Covers: using aliases vs typedef; readability for pointers/functions

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <vector>

namespace {

// Modern style
using IntVec = std::vector<int>;
using Str = std::string;
using FuncPtr = int (*)(int, int);

// Old style (still valid)
typedef unsigned long ulong_t;
typedef int (*OldFuncPtr)(int, int);

int add(int a, int b) {
    return a + b;
}

void demo_basics() {
    IntVec v{1, 2, 3};
    LEARN_CHECK(v.size() == 3);
    Str s = "alias";
    LEARN_CHECK(s == "alias");

    ulong_t u = 10ul;
    LEARN_CHECK(u == 10ul);

    static_assert(std::is_same_v<IntVec, std::vector<int>>);
    static_assert(std::is_same_v<Str, std::string>);
}

void demo_intermediate() {
    FuncPtr fp = add;
    OldFuncPtr ofp = add;
    LEARN_CHECK(fp(2, 3) == 5);
    LEARN_CHECK(ofp(4, 5) == 9);

    // using is clearer for function pointers and references
    using IntRef = int&;
    int x = 7;
    IntRef r = x;
    r = 8;
    LEARN_CHECK(x == 8);

    using CString = const char*;
    CString msg = "ok";
    LEARN_CHECK(msg[0] == 'o');
}

void demo_expert() {
    // Nested aliases in templates-like local scopes
    using MapKey = int;
    using MapVal = Str;
    static_assert(std::is_same_v<MapKey, int>);
    static_assert(std::is_same_v<MapVal, std::string>);

    // Alias for complex container
    using Table = std::vector<std::vector<int>>;
    Table t{{1, 2}, {3, 4}};
    LEARN_CHECK(t[1][0] == 3);

    // typedef cannot be templated; using can (see alias_template topic)
    // Here show equivalence of typedef and using for non-templates
    using ULong = unsigned long;
    static_assert(std::is_same_v<ULong, ulong_t>);

    FuncPtr ops[1] = {add};
    LEARN_CHECK(ops[0](10, 1) == 11);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/using_alias_replaces_typedef", run>;

}  // namespace
