// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : range_adaptor_closure_cpp23
// Topic id : part2/stage10/section08/range_adaptor_closure_cpp23
// Reference: WG21 P2387R3 (pipe support for user-defined range adaptors).

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

#include <vector>

namespace {

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
class multiply_by : public std::ranges::range_adaptor_closure<multiply_by> {
public:
    explicit multiply_by(int factor) : factor_(factor) {}

    template <std::ranges::viewable_range Range>
    auto operator()(Range&& range) const {
        return std::views::all(std::forward<Range>(range)) |
               std::views::transform([factor = factor_](int value) { return value * factor; });
    }

private:
    int factor_;
};

std::vector<int> collect(auto&& range) {
    std::vector<int> result;
    for (int value : range) {
        result.push_back(value);
    }
    return result;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/range_adaptor_closure_cpp23";

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
    learn::ExampleChecks checks{topic};
    std::vector values{1, 2, 3, 4, 5};

    const auto times_three = multiply_by{3};
    const auto first_two_times_three = times_three | std::views::take(2);
    const auto pipeline = values | first_two_times_three;

    LEARN_EXPECT_EQ(checks, collect(pipeline), (std::vector{3, 6}));
    values[0] = 10;  // The view is lazy and still refers to the named source.
    LEARN_EXPECT_EQ(checks, collect(pipeline), (std::vector{30, 6}));

    // Construction is O(1); work is proportional only to the elements consumed.
#if 0
    // A closure must still receive a viewable_range; an integer is not pipeable.
    const auto ill_formed = 42 | times_three;
#endif
    return checks.result();
#else
    learn::ExampleChecks checks{topic};
    const std::vector values{1, 2};
    std::vector<int> modeled;
    for (int value : values) {
        modeled.push_back(value * 3);
    }
    LEARN_EXPECT_EQ(checks, modeled, (std::vector{3, 6}));
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges >= 202202L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/range_adaptor_closure_cpp23", run>;

}  // namespace
