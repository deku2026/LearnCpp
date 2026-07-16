// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : ranges_expert_pitfalls
// Topic id : part2/stage10/section08/ranges_expert_pitfalls
// Reference: C++ working draft [range.view], [range.dangling], [range.req].

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
#if __has_include(<algorithm>)
#include <algorithm>
#endif

#include <concepts>
#include <sstream>
#include <vector>

namespace {

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 201911L
struct is_even {
    bool operator()(int value) const { return value % 2 == 0; }
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/ranges_expert_pitfalls";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 201911L
    std::vector source{1, 2, 3, 4, 5};
    int predicate_calls = 0;
    auto lazy = source | std::views::filter([&](int value) {
                    ++predicate_calls;
                    return value % 2 == 0;
                }) |
                std::views::take(1);
    LEARN_EXPECT_EQ(checks, predicate_calls, 0);
    LEARN_EXPECT_EQ(checks, *lazy.begin(), 2);
    LEARN_EXPECT_EQ(checks, predicate_calls, 2);

    using filter_type = decltype(std::declval<std::vector<int>&>() | std::views::filter(is_even{}));
    using temporary_find = decltype(std::ranges::find(std::vector{1, 2}, 2));
    static_assert(std::ranges::random_access_range<std::vector<int>&>);
    static_assert(!std::ranges::random_access_range<filter_type>);
    static_assert(std::same_as<temporary_find, std::ranges::dangling>);
    static_assert(!std::ranges::borrowed_range<std::vector<int>>);

    // views::all owns a movable rvalue range; it is not the classic dangling case.
    auto owning = std::views::all(std::vector{7, 8, 9});
    LEARN_EXPECT_EQ(checks, *owning.begin(), 7);

    std::istringstream input{"10 20"};
    auto single_pass = std::ranges::istream_view<int>(input);
    static_assert(std::ranges::input_range<decltype(single_pass)>);
    static_assert(!std::ranges::forward_range<decltype(single_pass)>);
    LEARN_EXPECT_EQ(checks, *single_pass.begin(), 10);

#if 0
    // Returning this view leaks references to a destroyed local vector.
    auto bad_factory() {
        std::vector local{1, 2, 3};
        return local | std::views::filter(is_even{});
    }
    // A view is not a container: arbitrary iterator invalidation rules still come from its base.
    lazy.push_back(6);
#endif
#else
    LEARN_EXPECT(checks, true);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "C++20 <ranges> support");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/ranges_expert_pitfalls", run>;

}  // namespace
