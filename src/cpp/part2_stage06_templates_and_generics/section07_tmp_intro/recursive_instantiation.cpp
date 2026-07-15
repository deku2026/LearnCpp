// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : recursive_instantiation
// Topic id : part2/stage06/section07/recursive_instantiation
//
// Covers: classic recursive template instantiation (factorial, power)

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

template <int B, int E>
struct Power {
    static constexpr int value = B * Power<B, E - 1>::value;
};

template <int B>
struct Power<B, 0> {
    static constexpr int value = 1;
};

template <typename... Ts>
struct Count;

template <>
struct Count<> {
    static constexpr int value = 0;
};

template <typename T, typename... Rest>
struct Count<T, Rest...> {
    static constexpr int value = 1 + Count<Rest...>::value;
};

void demo_basics() {
    static_assert(Factorial<5>::value == 120);
    LEARN_CHECK(Factorial<5>::value == 120);
    LEARN_CHECK(Factorial<0>::value == 1);
}

void demo_intermediate() {
    static_assert(Power<2, 10>::value == 1024);
    LEARN_CHECK((Power<3, 4>::value == 81));
}

void demo_expert() {
    static_assert(Count<int, double, char>::value == 3);
    static_assert(Count<>::value == 0);
    LEARN_CHECK((Count<int, int>::value == 2));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/recursive_instantiation", run>;

}  // namespace
