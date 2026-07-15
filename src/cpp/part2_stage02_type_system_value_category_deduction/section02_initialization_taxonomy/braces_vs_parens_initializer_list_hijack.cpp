// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : braces_vs_parens_initializer_list_hijack
// Topic id : part2/stage02/section02/braces_vs_parens_initializer_list_hijack
//
// Covers: {} prefers initializer_list overloads; () uses other constructors

#include "learn/topic_registry.hpp"

#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

namespace {

struct Tracker {
    enum Kind { FromSize, FromList, FromTwo };
    Kind kind{};
    int a{};
    int b{};

    Tracker(std::size_t n) : kind(FromSize), a(static_cast<int>(n)) {}
    Tracker(std::initializer_list<int> il) : kind(FromList), a(static_cast<int>(il.size())) {
        if (il.size() >= 1) {
            b = *il.begin();
        }
    }
    Tracker(int x, int y) : kind(FromTwo), a(x), b(y) {}
};

void demo_basics() {
    // Classic vector pitfall
    std::vector<int> by_size(3);  // 3 default elements
    std::vector<int> by_list{3};  // one element: 3
    assert(by_size.size() == 3);
    assert(by_list.size() == 1);
    assert(by_list[0] == 3);
}

void demo_intermediate() {
    Tracker t1(5);     // size ctor
    Tracker t2{5};     // initializer_list ctor hijacks
    Tracker t3(1, 2);  // two-arg ctor
    Tracker t4{1, 2};  // initializer_list of two ints

    assert(t1.kind == Tracker::FromSize);
    assert(t1.a == 5);

    assert(t2.kind == Tracker::FromList);
    assert(t2.a == 1);
    assert(t2.b == 5);

    assert(t3.kind == Tracker::FromTwo);
    assert(t3.a == 1 && t3.b == 2);

    assert(t4.kind == Tracker::FromList);
    assert(t4.a == 2);
    assert(t4.b == 1);
}

void demo_expert() {
    // string: count+char vs initializer_list-like list of chars via string
    std::string repeated(3, 'x');
    assert(repeated == "xxx");

    std::string listed{'a', 'b', 'c'};
    assert(listed == "abc");

    // vector of strings: braces create list of strings, not size
    std::vector<std::string> names{"ann", "bob"};
    assert(names.size() == 2);

    // When you want size with braces-friendly types, construct explicitly.
    std::vector<int> sized = std::vector<int>(4);
    assert(sized.size() == 4);

    // Empty braces value-init / default container
    std::vector<int> empty{};
    assert(empty.empty());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage02/section02/braces_vs_parens_initializer_list_hijack", run>;

}  // namespace
