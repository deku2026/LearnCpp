// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_as_const_cpp23
// Topic id : part2/stage10/section08/views_as_const_cpp23
// Reference: WG21 P2278R4 (views::as_const).

#include "learn/example_support.hpp"

#if __has_include(<ranges>)
#include <ranges>
#define LEARN_HAS_RANGES_HEADER 1
#else
#define LEARN_HAS_RANGES_HEADER 0
#endif

#include <concepts>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_as_const_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector values{1, 2, 3};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_as_const) && __cpp_lib_ranges_as_const >= 202207L
    auto read_only = values | std::views::as_const;
    static_assert(std::same_as<std::ranges::range_reference_t<decltype(read_only)>, const int&>);
    static_assert(std::ranges::random_access_range<decltype(read_only)>);
    LEARN_EXPECT_EQ(checks, read_only[1], 2);

    values[1] = 20;
    LEARN_EXPECT_EQ(checks, read_only[1], 20);
    // This is a non-owning, lazy access policy; it does not freeze or copy the source.
#if 0
    read_only[0] = 99;  // Ill-formed: the exposed reference is const int&.
#endif
#else
    const std::vector<int>& modeled = values;
    LEARN_EXPECT_EQ(checks, modeled[1], 2);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_as_const >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_as_const_cpp23", run>;

}  // namespace
