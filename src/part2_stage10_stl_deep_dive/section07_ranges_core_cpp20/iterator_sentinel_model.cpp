// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : iterator_sentinel_model
// Topic id : part2/stage10/section07/iterator_sentinel_model
//

#include "learn/example_support.hpp"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <ranges>
#include <string>

namespace {

struct NullSentinel {};

[[nodiscard]] constexpr bool operator==(const char* iterator, NullSentinel) noexcept {
    return *iterator == '\0';
}

struct NullTerminatedText {
    const char* data;

    [[nodiscard]] constexpr const char* begin() const noexcept { return data; }
    [[nodiscard]] constexpr NullSentinel end() const noexcept { return {}; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/iterator_sentinel_model"};
    static_assert(std::sentinel_for<NullSentinel, const char*>);
    static_assert(std::ranges::input_range<NullTerminatedText>);
    static_assert(!std::ranges::common_range<NullTerminatedText>);
    static_assert(
        !std::same_as<std::ranges::iterator_t<NullTerminatedText>, std::ranges::sentinel_t<NullTerminatedText>>);

    constexpr NullTerminatedText text{"range"};
    std::string materialized;
    for (const char character : text) {
        materialized.push_back(character);
    }

    const auto found = std::ranges::find(text, 'g');
    LEARN_EXPECT_EQ(checks, materialized, std::string{"range"});
    LEARN_EXPECT_EQ(checks, std::ranges::distance(text), std::ptrdiff_t{5});
    LEARN_EXPECT(checks, found != text.end());
    LEARN_EXPECT_EQ(checks, *found, 'g');

#if 0
    // The classic overload requires two iterators of one type, so this is ill-formed.
    std::find(text.begin(), text.end(), 'g');
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/iterator_sentinel_model", run>;

}  // namespace
