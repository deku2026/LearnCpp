// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : list_initialization
// Topic id : part2/stage02/section02/list_initialization
//
// Covers: direct/copy list-init, initializer_list, narrowing ban

#include "learn/topic_registry.hpp"

#include <initializer_list>
#include <string>
#include <vector>

namespace {

struct Agg {
    int x;
    int y;
};

struct FromList {
    std::vector<int> data;
    FromList(std::initializer_list<int> il) : data(il) {}
};

void demo_basics() {
    int a{1};
    int b = {2};
    LEARN_CHECK(a == 1);
    LEARN_CHECK(b == 2);

    Agg p{3, 4};
    LEARN_CHECK(p.x == 3 && p.y == 4);

    Agg q = {5, 6};
    LEARN_CHECK(q.x == 5 && q.y == 6);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK(v.size() == 4);
    LEARN_CHECK(v[0] == 1 && v[3] == 4);

    FromList fl{10, 20, 30};
    LEARN_CHECK(fl.data.size() == 3);
    LEARN_CHECK(fl.data[1] == 20);

    std::initializer_list<int> il = {7, 8, 9};
    LEARN_CHECK(il.size() == 3);

    std::string s{"list"};
    LEARN_CHECK(s == "list");
}

void demo_expert() {
    // Nested list initialization
    std::vector<std::vector<int>> matrix{{1, 2}, {3, 4}};
    LEARN_CHECK(matrix.size() == 2);
    LEARN_CHECK(matrix[0][1] == 2);
    LEARN_CHECK(matrix[1][0] == 3);

    // Empty list value-initializes many types
    int z{};
    Agg empty{};
    LEARN_CHECK(z == 0);
    LEARN_CHECK(empty.x == 0 && empty.y == 0);

    // Prefer list-init for containers when listing elements
    const std::vector<std::string> names{"ann", "bob"};
    LEARN_CHECK(names.size() == 2);
    LEARN_CHECK(names[0] == "ann");

    // Single-element list for scalars
    double d{1.25};
    LEARN_CHECK(d == 1.25);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/list_initialization", run>;

}  // namespace
