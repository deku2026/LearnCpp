// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_to_underlying_cpp23
// Topic id : part2/stage10/section05/std_to_underlying_cpp23
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <cstdint>
#include <string_view>
#include <type_traits>
#include <utility>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_to_underlying_cpp23";

enum class Permission : std::uint8_t { read = 1, write = 2 };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying >= 202102L
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::same_as<decltype(std::to_underlying(Permission::read)), std::uint8_t>);
    LEARN_EXPECT_EQ(checks, std::to_underlying(Permission::write), std::uint8_t{2});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::to_underlying");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_to_underlying_cpp23", run>;

}  // namespace
