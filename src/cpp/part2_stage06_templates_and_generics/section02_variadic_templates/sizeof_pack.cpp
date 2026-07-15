// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : sizeof_pack
// Topic id : part2/stage06/section02/sizeof_pack
//
// Covers: sizeof...(pack) for type and value packs

#include "learn/topic_registry.hpp"

#include <cstddef>

namespace {

template <typename... Ts>
constexpr std::size_t count_types() {
    return sizeof...(Ts);
}

template <typename... Ts>
constexpr std::size_t count_args(Ts...) {
    return sizeof...(Ts);
}

template <int... Ns>
constexpr int sum_sizeof_pack() {
    // sizeof...(Ns) is count of elements, not sum of values.
    return static_cast<int>(sizeof...(Ns));
}

void demo_basics() {
    static_assert(count_types<int, double>() == 2);
    static_assert(count_types<>() == 0);
    LEARN_CHECK(count_args(1, 2, 3) == 3);
    LEARN_CHECK(count_args() == 0);
}

void demo_intermediate() {
    LEARN_CHECK(count_args("a", "b") == 2);
    static_assert(sum_sizeof_pack<1, 2, 3, 4>() == 4);
    static_assert(sum_sizeof_pack<>() == 0);
}

void demo_expert() {
    auto f = [](auto... xs) { return sizeof...(xs); };
    LEARN_CHECK(f(1, 2, 3, 4, 5) == 5);

    auto g = [](auto head, auto... tail) { return 1 + static_cast<int>(sizeof...(tail)); };
    LEARN_CHECK(g(10, 20, 30) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/sizeof_pack", run>;

}  // namespace
