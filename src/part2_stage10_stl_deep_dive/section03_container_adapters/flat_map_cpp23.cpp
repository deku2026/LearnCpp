// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : flat_map_cpp23
// Topic id : part2/stage10/section03/flat_map_cpp23
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<flat_map>)
#include <flat_map>
#endif
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/flat_map_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_flat_map) && __cpp_lib_flat_map >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    std::flat_map<std::string, int> index{{"beta", 2}, {"alpha", 1}};
    index.try_emplace("gamma", 3);
    LEARN_EXPECT_EQ(checks, index.begin()->first, std::string{"alpha"});
    LEARN_EXPECT_EQ(checks, index.at("gamma"), 3);
    LEARN_EXPECT(checks, index.contains("beta"));
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::flat_map");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/flat_map_cpp23", run>;

}  // namespace
