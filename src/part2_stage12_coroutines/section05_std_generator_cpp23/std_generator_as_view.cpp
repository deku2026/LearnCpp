// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : std_generator_as_view
// Topic id : part2/stage12/section05/std_generator_as_view

#ifdef __has_include
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <ranges>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section05/std_generator_as_view";

template <std::ranges::input_range Range>
[[nodiscard]] std::vector<int> collect(Range&& range) {
    std::vector<int> result;
    for (const int value : range) {
        result.push_back(value);
    }
    return result;
}

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> naturals(int* produced) {
    for (int value = 1;; ++value) {
        ++*produced;
        co_yield value;
    }
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};
    const std::vector expected{4, 16, 36, 64, 100};

    auto fallback = std::views::iota(1) | std::views::filter([](int value) { return value % 2 == 0; }) |
                    std::views::transform([](int value) { return value * value; }) | std::views::take(5);
    LEARN_EXPECT_EQ(checks, collect(fallback), expected);

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    static_assert(std::ranges::view<std::generator<int>>);
    static_assert(std::ranges::input_range<std::generator<int>>);
    static_assert(!std::ranges::forward_range<std::generator<int>>);  // It is single-pass.

    int produced{};
    auto pipeline = naturals(&produced) | std::views::filter([](int value) { return value % 2 == 0; }) |
                    std::views::transform([](int value) { return value * value; }) | std::views::take(5);
    LEARN_EXPECT_EQ(checks, collect(pipeline), expected);
    LEARN_EXPECT(checks, produced >= 10 && produced <= 12);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_generator >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/std_generator_as_view", run>;

}  // namespace
