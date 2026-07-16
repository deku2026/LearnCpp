// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_multiset_cpp23
// Topic id : part2/stage10/section03/flat_multiset_cpp23
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<flat_set>)
#include <flat_set>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/flat_multiset_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::flat_multiset<int> values{3, 1, 3, 2};
    LEARN_EXPECT_EQ(checks, values.count(3), 2U);
    LEARN_EXPECT_EQ(checks, *values.begin(), 1);
    LEARN_EXPECT_EQ(checks, *values.rbegin(), 3);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::flat_multiset");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/flat_multiset_cpp23", run>;

}  // namespace
