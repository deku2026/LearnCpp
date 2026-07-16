// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multimap_cpp23
// Topic id : part2/stage10/section03/flat_multimap_cpp23
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<flat_map>)
#include <flat_map>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/flat_multimap_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::flat_multimap<std::string, int> grades{{"Ada", 90}, {"Ada", 95}, {"Lin", 88}};
    const auto [first, last] = grades.equal_range("Ada");
    LEARN_EXPECT_EQ(checks, std::distance(first, last), 2);
    LEARN_EXPECT_EQ(checks, first->second, 90);
    LEARN_EXPECT_EQ(checks, grades.count("Lin"), 1U);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::flat_multimap");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/flat_multimap_cpp23", run>;

}  // namespace
