// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_iterator_range_constructor_cpp23
// Topic id : part2/stage10/section06/string_view_iterator_range_constructor_cpp23

#ifdef __has_include
#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <concepts>
#include <iterator>
#include <memory>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_view_iterator_range_constructor_cpp23";

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L

// P1989 has no dedicated feature-test macro. Probe the constrained constructor itself.
template <class Iterator, class Sentinel>
concept NativeStringViewRangeConstructor = requires(Iterator first, Sentinel last) { std::string_view{first, last}; };

template <std::contiguous_iterator Iterator, std::sized_sentinel_for<Iterator> Sentinel>
    requires NativeStringViewRangeConstructor<Iterator, Sentinel>
[[nodiscard]] std::string_view make_view(Iterator first, Sentinel last) {
    return std::string_view{first, last};
}

template <std::contiguous_iterator Iterator, std::sized_sentinel_for<Iterator> Sentinel>
    requires(!NativeStringViewRangeConstructor<Iterator, Sentinel>)
[[nodiscard]] std::string_view make_view(Iterator first, Sentinel last) {
    const auto length = static_cast<std::size_t>(last - first);
    return {std::to_address(first), length};
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L
    learn::ExampleChecks checks{topic_id};
    const std::vector letters{'r', 'a', 'n', 'g', 'e', '-', 'v', 'i', 'e', 'w'};
    auto first = letters.cbegin() + 6;
    auto last = letters.cend();
    const auto view = make_view(first, last);

    LEARN_EXPECT_EQ(checks, view, "view");
    LEARN_EXPECT_EQ(checks, view.data(), letters.data() + 6);
    static_assert(std::contiguous_iterator<decltype(first)>);

    using Iterator = decltype(first);
    if constexpr (NativeStringViewRangeConstructor<Iterator, Iterator>) {
        return checks.result();
    } else {
        if (checks.result() != 0) {
            return checks.result();
        }
        return learn::ExampleChecks::unavailable(topic_id, "C++23 string_view iterator-pair constructor");
    }
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::string_view");
#endif
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06/string_view_iterator_range_constructor_cpp23", run>;

}  // namespace
