// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_bool_pitfall
// Topic id : part2/stage10/section01/vector_bool_pitfall
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/vector_bool_pitfall";

using BitReference = std::vector<bool>::reference;
static_assert(!std::same_as<BitReference, bool&>);
static_assert(std::convertible_to<BitReference, bool>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<bool> bits{false, true};
    auto proxy = bits[0];
    proxy = true;
    const bool ordinary_value = bits[1];
    LEARN_EXPECT(checks, bits[0]);
    LEARN_EXPECT(checks, ordinary_value);
    bits.flip();
    LEARN_EXPECT(checks, !bits[0] && !bits[1]);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/vector_bool_pitfall", run>;

}  // namespace
