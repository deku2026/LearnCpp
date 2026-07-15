// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : comparison_operators_cpp20_spaceship
// Topic id : part2/stage04/section05/comparison_operators_cpp20_spaceship
//
// Covers: operator<=> rewrite, ordering categories, custom <=> needs ==

#include "learn/topic_registry.hpp"

#include <compare>
#include <string>

namespace {

struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;
    auto operator<=>(const Version&) const = default;
};

struct Rank {
    int score = 0;
    std::strong_ordering operator<=>(const Rank& o) const { return score <=> o.score; }
    bool operator==(const Rank& o) const { return score == o.score; }
};

void demo_basics() {
    Version a{1, 2, 0};
    Version b{1, 3, 0};
    LEARN_CHECK(a < b);
    LEARN_CHECK(!(a == b));
    LEARN_CHECK(a != b);
    LEARN_CHECK(b >= a);
}

void demo_intermediate() {
    LEARN_CHECK((4 <=> 5) < 0);
    LEARN_CHECK((5 <=> 5) == 0);
    LEARN_CHECK((6 <=> 5) > 0);
}

void demo_expert() {
    Rank a{10};
    Rank b{20};
    LEARN_CHECK(a < b);
    LEARN_CHECK(a != b);
    // Hand-written <=> does not synthesize ==; we provided == separately.
    LEARN_CHECK((a <=> b) == std::strong_ordering::less);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/comparison_operators_cpp20_spaceship", run>;

}  // namespace
