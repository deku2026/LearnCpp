// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : defaulted_comparison_operators
// Topic id : part2/stage04/section05/defaulted_comparison_operators
//
// Covers: =default <=> and == generate full comparison set

#include "learn/topic_registry.hpp"

#include <compare>
#include <string>

namespace {

struct Person {
    std::string name;
    int id = 0;
    auto operator<=>(const Person&) const = default;
};

struct OnlyEq {
    int x = 0;
    bool operator==(const OnlyEq&) const = default;
};

void demo_basics() {
    Person a{"Ada", 1};
    Person b{"Ada", 2};
    Person c{"Bob", 1};
    LEARN_CHECK(a < b);
    LEARN_CHECK(a != b);
    LEARN_CHECK(a < c);
}

void demo_intermediate() {
    OnlyEq a{1};
    OnlyEq b{1};
    OnlyEq c{2};
    LEARN_CHECK(a == b);
    LEARN_CHECK(a != c);
}

void demo_expert() {
    Person a{"Z", 9};
    Person b = a;
    LEARN_CHECK(a == b);
    LEARN_CHECK((a <=> b) == 0);
    // Lexicographic: base then members in declaration order.
    Person c{"Z", 10};
    LEARN_CHECK(a < c);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/defaulted_comparison_operators", run>;

}  // namespace
