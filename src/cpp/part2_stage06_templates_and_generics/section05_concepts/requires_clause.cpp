// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : requires_clause
// Topic id : part2/stage06/section05/requires_clause
//
// Covers: requires-clause on function/class templates

#include "learn/topic_registry.hpp"

#include <concepts>
#include <type_traits>

namespace {

template <typename T>
    requires std::integral<T>
T twice(T x) {
    return static_cast<T>(x * 2);
}

template <typename T>
    requires std::floating_point<T>
T twice(T x) {
    return x * 2.0;
}

template <typename T>
    requires(sizeof(T) >= 4)
constexpr int min_bytes() {
    return static_cast<int>(sizeof(T));
}

template <typename T>
    requires std::totally_ordered<T>
bool less_eq(const T& a, const T& b) {
    return a <= b;
}

void demo_basics() {
    LEARN_CHECK(twice(21) == 42);
    LEARN_CHECK(twice(1.5) == 3.0);
}

void demo_intermediate() {
    static_assert(min_bytes<int>() >= 4);
    LEARN_CHECK(min_bytes<int>() >= 4);
    LEARN_CHECK(less_eq(1, 2));
    LEARN_CHECK(less_eq(2.0, 2.0));
}

void demo_expert() {
    // Ad-hoc requires expression in clause.
    auto mid = [](auto a, auto b)
        requires std::same_as<decltype(a), decltype(b)> && std::integral<decltype(a)>
    { return (a + b) / 2; };
    LEARN_CHECK(mid(2, 6) == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/requires_clause", run>;

}  // namespace
