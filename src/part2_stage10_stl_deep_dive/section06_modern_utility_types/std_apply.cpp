// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_apply
// Topic id : part2/stage10/section06/std_apply

#ifdef __has_include
#if __has_include(<tuple>)
#include <tuple>
#define LEARN_HAS_TUPLE_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <string>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_apply";

#if defined(LEARN_HAS_TUPLE_HEADER) && defined(__cpp_lib_apply) && __cpp_lib_apply >= 201603L

template <class Tuple>
[[nodiscard]] constexpr auto sum_tuple(Tuple&& tuple) {
    return std::apply([]<class... Values>(Values&&... values) { return (std::forward<Values>(values) + ...); },
                      std::forward<Tuple>(tuple));
}

struct Price {
    int cents;

    [[nodiscard]] int with_tax(int percent) const noexcept { return cents + cents * percent / 100; }
};

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_TUPLE_HEADER) && defined(__cpp_lib_apply) && __cpp_lib_apply >= 201603L
    learn::ExampleChecks checks{topic_id};

    constexpr auto numbers = std::tuple{1, 2, 3, 4};
    static_assert(sum_tuple(numbers) == 10);

    const auto label = std::apply(
        [](std::string_view name, int version, bool stable) {
            return std::string{name} + '-' + std::to_string(version) + (stable ? "-stable" : "-preview");
        },
        std::tuple{"api", 23, true});
    LEARN_EXPECT_EQ(checks, label, "api-23-stable");

    const Price price{2'000};
    const auto member_arguments = std::tuple{&price, 20};
    LEARN_EXPECT_EQ(checks, std::apply(&Price::with_tax, member_arguments), 2'400);

    int left = 5;
    int right = 7;
    std::apply([](int& a, int& b) { std::swap(a, b); }, std::tie(left, right));
    LEARN_EXPECT_EQ(checks, left, 7);
    LEARN_EXPECT_EQ(checks, right, 5);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 __cpp_lib_apply >= 201603L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_apply", run>;

}  // namespace
