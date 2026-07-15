// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : auto_nttp_cpp17
// Topic id : part2/stage06/section03/auto_nttp_cpp17
//
// Covers: auto non-type template parameters (C++17)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

template <auto V>
constexpr auto value() {
    return V;
}

template <auto V>
struct Constant {
    static constexpr auto value = V;
};

template <auto A, auto B>
constexpr auto add_const() {
    return A + B;
}

void demo_basics() {
    static_assert(value<42>() == 42);
    static_assert(value<true>() == true);
    assert(value<'Z'>() == 'Z');
}

void demo_intermediate() {
    static_assert(Constant<10>::value == 10);
    static_assert(std::is_same_v<decltype(Constant<10>::value), const int>);
    static_assert(std::is_same_v<decltype(Constant<3u>::value), const unsigned>);
    assert((add_const<2, 3>() == 5));
}

void demo_expert() {
    // Different values => different types for Constant.
    static_assert(!std::is_same_v<Constant<1>, Constant<2>>);
    static_assert(add_const<1, 2>() + add_const<3, 4>() == 10);
    static_assert(value<'A'>() == 'A');
    assert(value<false>() == false);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/auto_nttp_cpp17", run>;

}  // namespace
