// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : standard_concepts_library
// Topic id : part2/stage06/section05/standard_concepts_library
//
// Covers: std::integral, same_as, convertible_to, invocable, ranges concepts

#include "learn/topic_registry.hpp"

#include <concepts>
#include <string>
#include <vector>

namespace {

template <std::integral T>
T inc(T x) {
    return static_cast<T>(x + 1);
}

template <typename T, typename U>
    requires std::same_as<T, U>
bool same_values(T a, U b) {
    return a == b;
}

template <std::invocable<int> F>
int call_with_one(F f) {
    return f(1);
}

void demo_basics() {
    static_assert(std::integral<int>);
    static_assert(std::signed_integral<int>);
    static_assert(std::unsigned_integral<unsigned>);
    LEARN_CHECK(inc(41) == 42);
}

void demo_intermediate() {
    static_assert(std::same_as<int, int>);
    static_assert(std::convertible_to<int, double>);
    LEARN_CHECK(same_values(3, 3));
    LEARN_CHECK(call_with_one([](int x) { return x + 9; }) == 10);
}

void demo_expert() {
    static_assert(std::totally_ordered<int>);
    static_assert(std::equality_comparable<std::string>);
    static_assert(std::default_initializable<std::vector<int>>);
    static_assert(std::movable<std::string>);
    static_assert(std::copyable<int>);
    LEARN_CHECK(std::regular<int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/standard_concepts_library", run>;

}  // namespace
