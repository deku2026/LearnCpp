// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_zip_transform_cpp23
// Topic id : part2/stage10/section08/views_zip_transform_cpp23
// Reference: WG21 P2321R2 (__cpp_lib_ranges_zip covers zip_transform).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<ranges>)
#include <ranges>
#define LEARN_HAS_RANGES_HEADER 1
#else
#define LEARN_HAS_RANGES_HEADER 0
#endif

#include <algorithm>
#include <functional>
#include <vector>

namespace {

std::vector<int> collect(auto&& range) {
    std::vector<int> result;
    for (int value : range) {
        result.push_back(value);
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_zip_transform_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector left{1, 2, 3};
    std::vector right{10, 20};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    int calls = 0;
    auto sums = std::views::zip_transform(
        [&](int lhs, int rhs) {
            ++calls;
            return lhs + rhs;
        },
        left, right);
    LEARN_EXPECT_EQ(checks, calls, 0);
    LEARN_EXPECT_EQ(checks, collect(sums), (std::vector{11, 22}));
    LEARN_EXPECT_EQ(checks, calls, 2);

    left[0] = 5;
    LEARN_EXPECT_EQ(checks, *sums.begin(), 15);
    std::vector<int> empty;
    LEARN_EXPECT(checks, std::views::zip_transform(std::plus<>{}, left, empty).empty());
    // Transform calls occur on dereference, and traversal stops at the shortest input.
#else
    std::vector<int> modeled;
    const auto count = (std::min)(left.size(), right.size());
    for (std::size_t index = 0; index < count; ++index) {
        modeled.push_back(left[index] + right[index]);
    }
    LEARN_EXPECT_EQ(checks, modeled, (std::vector{11, 22}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_zip >= 202110L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_zip_transform_cpp23", run>;

}  // namespace
