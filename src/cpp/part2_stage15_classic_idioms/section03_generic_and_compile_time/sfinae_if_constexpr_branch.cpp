// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §3.2
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : sfinae_if_constexpr_branch
// Topic id : part2/stage15/section03/sfinae_if_constexpr_branch
//
// Evolution of compile-time selection: SFINAE → if constexpr → Concepts.
// Motive : pick different implementations by type properties without runtime cost.
// Modern : if constexpr for internal branches; Concepts for interface constraints.
// Pitfall: SFINAE is hard to debug; if constexpr still instantiates the chosen arm only
//          but discarded arm must be syntactically valid for the template.

#include "learn/topic_registry.hpp"

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// ---------- 1) SFINAE enable_if (legacy) ----------
template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
std::string describe_sfinae(T x) {
    return std::string{"integral "} + std::to_string(x);
}

template <typename T, std::enable_if_t<!std::is_integral_v<T>, int> = 0>
std::string describe_sfinae(T) {
    return "non-integral";
}

// SFINAE on return type
template <typename T>
auto serialize_sfinae(const T& v) -> std::enable_if_t<std::is_arithmetic_v<T>, std::string> {
    return std::to_string(v);
}

// ---------- 2) if constexpr (C++17) ----------
template <typename T>
std::string describe_constexpr(T x) {
    if constexpr (std::is_integral_v<T>) {
        return std::string{"integral "} + std::to_string(x);
    } else if constexpr (std::is_floating_point_v<T>) {
        return std::string{"float "} + std::to_string(x);
    } else {
        return "other";
    }
}

template <typename T>
auto maybe_size(const T& t) {
    if constexpr (requires { t.size(); }) {
        return t.size();
    } else {
        return std::size_t{0};
    }
}

// ---------- 3) Concepts (C++20) constrain the interface ----------
template <std::integral T>
std::string describe_concept(T x) {
    return std::string{"concept-integral "} + std::to_string(x);
}

template <typename T>
    requires(!std::integral<T>)
std::string describe_concept(T) {
    return "concept-non-integral";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== SFINAE enable_if ==\n";
    assert(describe_sfinae(42) == "integral 42");
    assert(describe_sfinae(3.14) == "non-integral");
    assert(serialize_sfinae(7) == "7");
    std::cout << "  " << describe_sfinae(42) << " / " << describe_sfinae(1.5) << '\n';

    std::cout << "== if constexpr ==\n";
    std::cout << "  " << describe_constexpr(9) << '\n';
    std::cout << "  " << describe_constexpr(1.25) << '\n';
    std::cout << "  " << describe_constexpr(std::string{"x"}) << '\n';
    assert(maybe_size(std::string{"abcd"}) == 4);
    assert(maybe_size(42) == 0);

    std::cout << "== Concepts ==\n";
    std::cout << "  " << describe_concept(3) << '\n';
    std::cout << "  " << describe_concept(std::string{"z"}) << '\n';

    std::cout << "guidance: new code → Concepts for APIs, if constexpr inside bodies;\n"
              << "          keep SFINAE literacy for reading pre-C++17 libraries\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/sfinae_if_constexpr_branch", run>;

}  // namespace
