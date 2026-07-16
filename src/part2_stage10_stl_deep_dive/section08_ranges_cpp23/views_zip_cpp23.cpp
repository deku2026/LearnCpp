// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_zip_cpp23
// Topic id : part2/stage10/section08/views_zip_cpp23
// Reference: WG21 P2321R2 (zip).

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
#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_zip_cpp23";
    learn::ExampleChecks checks{topic};
    std::vector<std::string> names{"Ada", "Bjarne", "Grace"};
    std::vector scores{90, 95};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
    auto zipped = std::views::zip(names, scores);
    std::vector<std::string> rows;
    for (auto [name, score] : zipped) {
        rows.push_back(name + '=' + std::to_string(score));
        score += 1;  // Tuple-like elements preserve references to their bases.
    }
    LEARN_EXPECT_EQ(checks, rows, (std::vector<std::string>{"Ada=90", "Bjarne=95"}));
    LEARN_EXPECT_EQ(checks, scores, (std::vector{91, 96}));
    LEARN_EXPECT_EQ(checks, zipped.size(), std::size_t{2});

    std::vector<int> empty;
    LEARN_EXPECT(checks, std::views::zip(names, empty).empty());
    // zip terminates at the shortest input; longer tails are deliberately ignored.
#else
    std::vector<std::string> rows;
    const auto count = (std::min)(names.size(), scores.size());
    for (std::size_t index = 0; index < count; ++index) {
        rows.push_back(names[index] + '=' + std::to_string(scores[index]));
    }
    LEARN_EXPECT_EQ(checks, rows.size(), std::size_t{2});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_zip >= 202110L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_zip_cpp23", run>;

}  // namespace
