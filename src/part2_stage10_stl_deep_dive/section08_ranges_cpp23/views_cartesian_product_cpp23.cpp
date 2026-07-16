// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : views_cartesian_product_cpp23
// Topic id : part2/stage10/section08/views_cartesian_product_cpp23
// Reference: WG21 P2374R4 (views::cartesian_product).

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

#include <array>
#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part2/stage10/section08/views_cartesian_product_cpp23";
    learn::ExampleChecks checks{topic};
    std::array letters{'A', 'B'};
    std::vector numbers{1, 2, 3};

#if LEARN_HAS_RANGES_HEADER && defined(__cpp_lib_ranges_cartesian_product) && \
    __cpp_lib_ranges_cartesian_product >= 202207L
    auto product = std::views::cartesian_product(letters, numbers);
    std::vector<std::string> labels;
    for (auto [letter, number] : product) {
        labels.push_back(std::string(1, letter) + std::to_string(number));
    }
    LEARN_EXPECT_EQ(checks, labels, (std::vector<std::string>{"A1", "A2", "A3", "B1", "B2", "B3"}));
    LEARN_EXPECT_EQ(checks, product.size(), std::size_t{6});

    std::vector<int> empty;
    LEARN_EXPECT(checks, std::views::cartesian_product(letters, empty).empty());
    // Cardinality is the product of factor sizes; iteration can therefore grow exponentially.
#else
    std::vector<std::string> labels;
    for (char letter : letters) {
        for (int number : numbers) {
            labels.push_back(std::string(1, letter) + std::to_string(number));
        }
    }
    LEARN_EXPECT_EQ(checks, labels.size(), std::size_t{6});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_ranges_cartesian_product >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section08/views_cartesian_product_cpp23", run>;

}  // namespace
