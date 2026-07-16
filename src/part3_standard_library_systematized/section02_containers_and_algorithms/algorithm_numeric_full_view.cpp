// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : algorithm_numeric_full_view
// Topic id : part3/section02/algorithm_numeric_full_view
// Reference: C++23 working draft [algorithms], [numeric.ops], [numeric.iota].

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/algorithm_numeric_full_view";
    learn::ExampleChecks checks{topic};

    // Generation and non-modifying queries.
    std::array<int, 5> values{};
    std::iota(values.begin(), values.end(), 1);
    LEARN_EXPECT(checks, std::ranges::all_of(values, [](int value) { return value > 0; }));
    LEARN_EXPECT_EQ(checks, std::ranges::count_if(values, [](int value) { return value % 2 != 0; }), std::ptrdiff_t{3});

    // Transform/copy algorithms write through an output iterator; they do not grow a container
    // unless an inserter such as back_inserter is used.
    std::vector<int> squares;
    squares.reserve(values.size());
    std::ranges::transform(values, std::back_inserter(squares), [](int value) { return value * value; });
    LEARN_EXPECT_EQ(checks, squares, std::vector<int>({1, 4, 9, 16, 25}));

    // Sorting/searching preconditions matter: binary_search and lower_bound require partitioned
    // (normally sorted) input under the same comparator and projection.
    std::vector<int> shuffled{5, 1, 4, 2, 3, 2};
    std::ranges::sort(shuffled);
    LEARN_EXPECT(checks, std::ranges::binary_search(shuffled, 4));
    const auto unique_tail = std::ranges::unique(shuffled);
    shuffled.erase(unique_tail.begin(), unique_tail.end());
    LEARN_EXPECT_EQ(checks, shuffled, std::vector<int>({1, 2, 3, 4, 5}));

    std::vector<int> odds{1, 3, 5, 7};
    std::vector<int> intersection;
    std::ranges::set_intersection(shuffled, odds, std::back_inserter(intersection));
    LEARN_EXPECT_EQ(checks, intersection, std::vector<int>({1, 3, 5}));

    // Numeric algorithms predate ranges in C++23. The initial value also fixes the accumulator
    // type, so use an explicitly wide seed when narrowing or overflow would be a risk.
    const auto sum = std::accumulate(values.begin(), values.end(), std::int64_t{0});
    const auto sum_reorderable = std::reduce(values.begin(), values.end(), std::int64_t{0});
    const std::array weights{5, 4, 3, 2, 1};
    const auto dot = std::inner_product(values.begin(), values.end(), weights.begin(), 0);
    std::array<int, 5> prefixes{};
    std::partial_sum(values.begin(), values.end(), prefixes.begin());
    LEARN_EXPECT_EQ(checks, sum, std::int64_t{15});
    LEARN_EXPECT_EQ(checks, sum_reorderable, std::int64_t{15});
    LEARN_EXPECT_EQ(checks, dot, 35);
    LEARN_EXPECT_EQ(checks, prefixes, (std::array{1, 3, 6, 10, 15}));

    // reduce may regroup operations (and execution-policy overloads may run them in parallel), so
    // use it only with operations for which regrouping is acceptable; accumulate is left-to-right.
#if 0
    const auto invalid = std::ranges::binary_search(std::vector{3, 1, 2}, 2);
    // The call compiles, but violates binary_search's partitioning precondition.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/algorithm_numeric_full_view", run>;

}  // namespace
