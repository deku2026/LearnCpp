// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : direct_initialization
// Topic id : part2/stage02/section02/direct_initialization
//
// Covers: T(args), T{args} direct forms, explicit constructors

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace {

struct ExplicitOnly {
    explicit ExplicitOnly(int v) : value(v) {}
    int value;
};

struct Multi {
    int a;
    int b;
    Multi(int x, int y) : a(x), b(y) {}
};

void demo_basics() {
    int n(42);
    double d(2.5);
    assert(n == 42);
    assert(d == 2.5);

    std::string s("hello");
    assert(s == "hello");

    ExplicitOnly e(10);  // direct-init allows explicit ctor
    assert(e.value == 10);
}

void demo_intermediate() {
    Multi m(1, 2);
    assert(m.a == 1 && m.b == 2);

    Multi m2{3, 4};  // direct-list-init also selects constructor
    assert(m2.a == 3 && m2.b == 4);

    std::vector<int> v(3, 9);  // 3 elements, each 9
    assert(v.size() == 3);
    assert(v[0] == 9 && v[2] == 9);

    // Functional cast style for scalars
    assert(int(3.9) == 3);
}

void demo_expert() {
    // Placement of parentheses vs braces with vector:
    // vector(size) vs vector{value} — different meanings.
    std::vector<int> sized(2);
    assert(sized.size() == 2);
    assert(sized[0] == 0 && sized[1] == 0);

    std::vector<int> listed{2};
    assert(listed.size() == 1);
    assert(listed[0] == 2);

    ExplicitOnly e{99};
    assert(e.value == 99);

    // Direct init of references is not the topic; show temporary binding pattern carefully.
    const std::string& ref = std::string("temp");
    assert(ref == "temp");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/direct_initialization", run>;

}  // namespace
