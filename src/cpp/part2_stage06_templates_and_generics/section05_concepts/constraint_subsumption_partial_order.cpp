// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : constraint_subsumption_partial_order
// Topic id : part2/stage06/section05/constraint_subsumption_partial_order
//
// Covers: more constrained overload wins via subsumption

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <type_traits>

namespace {

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <typename T>
concept IntegralNumber = std::integral<T>;

template <Number T>
int score(T) {
    return 1;
}

template <IntegralNumber T>
int score(T) {
    return 2;  // more constrained for integrals
}

template <typename T>
int rank(T) {
    return 0;
}

template <std::integral T>
int rank(T) {
    return 1;
}

template <std::signed_integral T>
int rank(T) {
    return 2;
}

void demo_basics() {
    assert(score(3.5) == 1);
    assert(score(3) == 2);
}

void demo_intermediate() {
    assert(rank(3.0) == 0);
    assert(rank(3u) == 1);
    assert(rank(3) == 2);
}

void demo_expert() {
    // signed_integral subsumes integral for signed types.
    static_assert(std::signed_integral<int>);
    static_assert(std::integral<int>);
    assert(rank(static_cast<short>(-1)) == 2);
    assert(rank(static_cast<unsigned short>(1)) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/constraint_subsumption_partial_order", run>;

}  // namespace
