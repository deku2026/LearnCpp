// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_get_and_index
// Topic id : part2/stage10/section06_modern_utility_types/std_get_and_index
//
// Covers: std::get by index and type for tuple/variant/array

#include "learn/topic_registry.hpp"

#include <array>
#include <tuple>
#include <variant>

namespace {

void demo_basics() {
    std::tuple<int, char> t{7, 'x'};
    LEARN_CHECK(std::get<0>(t) == 7);
    LEARN_CHECK(std::get<char>(t) == 'x');
}

void demo_intermediate() {
    std::array<int, 3> a{1, 2, 3};
    LEARN_CHECK(std::get<1>(a) == 2);
    std::variant<int, double> v = 3.5;
    LEARN_CHECK(std::get<double>(v) == 3.5);
}

void demo_expert() {
    std::tuple<int, int, int> t{1, 2, 3};
    std::get<2>(t) = 9;
    LEARN_CHECK(std::get<2>(t) == 9);
    constexpr auto a = std::array{10, 20};
    static_assert(std::get<0>(a) == 10);
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06_modern_utility_types/std_get_and_index", run>;

}  // namespace
