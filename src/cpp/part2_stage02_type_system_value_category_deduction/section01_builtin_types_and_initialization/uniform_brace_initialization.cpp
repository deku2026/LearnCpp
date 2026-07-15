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
    LEARN_CHECK(a == 42);
    LEARN_CHECK(b == 2.5);
    LEARN_CHECK(ok);
    LEARN_CHECK(zeros == 0);
}

void demo_intermediate() {
    int arr[3]{1, 2, 3};
    LEARN_CHECK(arr[0] == 1 && arr[1] == 2 && arr[2] == 3);

    int partial[4]{1, 2};  // remaining value-initialized
    LEARN_CHECK(partial[0] == 1 && partial[1] == 2);
    LEARN_CHECK(partial[2] == 0 && partial[3] == 0);

    Point p{10, 20};
    LEARN_CHECK(p.x == 10 && p.y == 20);

    Point origin{};
    LEARN_CHECK(origin.x == 0 && origin.y == 0);

    std::string s{"hello"};
    LEARN_CHECK(s == "hello");
}

void demo_expert() {
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK(v.size() == 4);
    LEARN_CHECK(v[0] == 1 && v[3] == 4);

    std::array<int, 3> a{7, 8, 9};
    LEARN_CHECK(a[0] == 7 && a[2] == 9);

    // Nested braces for nested aggregates
    struct Box {
        Point min;
        Point max;
    };
    Box box{{0, 0}, {10, 10}};
    LEARN_CHECK(box.min.x == 0 && box.max.y == 10);

    // Empty braces: value initialization for many types
    std::vector<int> empty{};
    LEARN_CHECK(empty.empty());

    int* p{};
    LEARN_CHECK(p == nullptr);
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
