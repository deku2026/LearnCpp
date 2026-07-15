// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : uniform_brace_initialization
// Topic id : part2/stage02/section01/uniform_brace_initialization
//
// Covers: uniform {} init for scalars, arrays, aggregates, empty braces

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <string>
#include <vector>

namespace {

struct Point {
    int x;
    int y;
};

void demo_basics() {
    int a{42};
    double b{2.5};
    bool ok{true};
    int zeros{};  // value-init -> 0
    assert(a == 42);
    assert(b == 2.5);
    assert(ok);
    assert(zeros == 0);
}

void demo_intermediate() {
    int arr[3]{1, 2, 3};
    assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 3);

    int partial[4]{1, 2};  // remaining value-initialized
    assert(partial[0] == 1 && partial[1] == 2);
    assert(partial[2] == 0 && partial[3] == 0);

    Point p{10, 20};
    assert(p.x == 10 && p.y == 20);

    Point origin{};
    assert(origin.x == 0 && origin.y == 0);

    std::string s{"hello"};
    assert(s == "hello");
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4};
    assert(v.size() == 4);
    assert(v[0] == 1 && v[3] == 4);

    std::array<int, 3> a{7, 8, 9};
    assert(a[0] == 7 && a[2] == 9);

    // Nested braces for nested aggregates
    struct Box {
        Point min;
        Point max;
    };
    Box box{{0, 0}, {10, 10}};
    assert(box.min.x == 0 && box.max.y == 10);

    // Empty braces: value initialization for many types
    std::vector<int> empty{};
    assert(empty.empty());

    int* p{};
    assert(p == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/uniform_brace_initialization", run>;

}  // namespace
