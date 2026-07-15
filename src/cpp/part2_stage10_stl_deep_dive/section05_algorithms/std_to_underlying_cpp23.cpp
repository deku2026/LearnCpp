// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_to_underlying_cpp23
// Topic id : part2/stage10/section05_algorithms/std_to_underlying_cpp23
//
// Covers: std::to_underlying C++23 enum to underlying integer

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <version>

#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
#include <utility>
#endif

namespace {

enum class Color : int { Red = 1, Green = 2, Blue = 3 };

void demo_basics() {
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    LEARN_CHECK(std::to_underlying(Color::Green) == 2);
#else
    LEARN_CHECK(static_cast<std::underlying_type_t<Color>>(Color::Green) == 2);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    static_assert(std::is_same_v<decltype(std::to_underlying(Color::Red)), int>);
    LEARN_CHECK(std::to_underlying(Color::Blue) == 3);
#else
    static_assert(std::is_same_v<std::underlying_type_t<Color>, int>);
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
    enum class Flags : unsigned { None = 0, A = 1, B = 2 };
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    auto v = std::to_underlying(Flags::A) | std::to_underlying(Flags::B);
    LEARN_CHECK(v == 3u);
#else
    auto v = static_cast<unsigned>(Flags::A) | static_cast<unsigned>(Flags::B);
    LEARN_CHECK(v == 3u);
#endif
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_to_underlying_cpp23", run>;

}  // namespace
