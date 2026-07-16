// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_chunk_cpp23
// Topic id : part2/stage10/section08/views_chunk_cpp23
// Reference: WG21 P2442R1 (views::chunk and views::slide).

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
#include <vector>

namespace {

std::vector<std::vector<int>> collect_chunks(auto&& chunks) {
    std::vector<std::vector<int>> result;
    for (auto chunk : chunks) {
        result.emplace_back(chunk.begin(), chunk.end());
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_chunk_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector values{1, 2, 3, 4, 5, 6, 7};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_chunk) && __cpp_lib_ranges_chunk >= 202202L
    auto chunks = values | std::views::chunk(3);
    LEARN_EXPECT_EQ(checks, collect_chunks(chunks), (std::vector<std::vector<int>>{{1, 2, 3}, {4, 5, 6}, {7}}));
    LEARN_EXPECT_EQ(checks, chunks.size(), std::size_t{3});

    values.back() = 70;
    LEARN_EXPECT_EQ(checks, collect_chunks(chunks).back(), (std::vector{70}));
    LEARN_EXPECT(checks, (std::vector<int>{} | std::views::chunk(3)).empty());
    // The last chunk may be short; count is ceil(N / width). Width must be positive.
#if 0
    const auto precondition_violation = values | std::views::chunk(0);
#endif
#else
    std::vector<std::vector<int>> modeled;
    for (std::size_t first = 0; first < values.size(); first += 3) {
        const auto last = (std::min)(first + 3, values.size());
        modeled.emplace_back(values.begin() + static_cast<std::ptrdiff_t>(first),
                             values.begin() + static_cast<std::ptrdiff_t>(last));
    }
    LEARN_EXPECT_EQ(checks, modeled.size(), std::size_t{3});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_chunk >= 202202L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_chunk_cpp23", run>;

}  // namespace
