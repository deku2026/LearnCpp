// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : concept_definition
// Topic id : part2/stage06/section05/concept_definition
//
// Covers: defining concepts with requires / compound requirements

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <string>
#include <type_traits>

namespace {

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <typename T>
concept HasSize = requires(const T& t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

template <Addable T>
T add_two(T a, T b) {
    return a + b;
}

template <HasSize T>
std::size_t size_of(const T& t) {
    return t.size();
}

void demo_basics() {
    static_assert(Addable<int>);
    static_assert(Addable<std::string>);
    assert(add_two(2, 3) == 5);
    assert(add_two(std::string{"a"}, std::string{"b"}) == "ab");
}

void demo_intermediate() {
    static_assert(HasSize<std::string>);
    assert(size_of(std::string{"xyz"}) == 3);
    static_assert(!HasSize<int>);
}

void demo_expert() {
    static_assert(std::integral<int>);
    static_assert(!std::integral<double>);
    assert(add_two(1.5, 2.5) == 4.0);
    static_assert(Addable<double>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/concept_definition", run>;

}  // namespace
