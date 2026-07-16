// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_std_vector_cpp20
// Topic id : part2/stage07/section05/constexpr_std_vector_cpp20
// References: N4950 [vector], [expr.const]; P1004R2, P0784R7.

#include "learn/example_support.hpp"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace {

#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L && \
    defined(__cpp_lib_constexpr_algorithms) && __cpp_lib_constexpr_algorithms >= 201806L

struct VectorStatistics {
    int sum;
    int median;
    std::size_t distinct_count;
    friend constexpr bool operator==(const VectorStatistics&, const VectorStatistics&) = default;
};

constexpr VectorStatistics analyze_values() {
    std::vector values{5, 1, 3, 3, 2, 5, 4};
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    int sum{};
    for (const int value : values) {
        sum += value;
    }
    return VectorStatistics{sum, values[values.size() / 2], values.size()};
}

static_assert(analyze_values() == VectorStatistics{15, 3, 5});

#if 0
// A vector whose dynamic storage survives the constant evaluation is rejected.
constexpr std::vector<int> persistent_vector{1, 2, 3};
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L && \
    defined(__cpp_lib_constexpr_algorithms) && __cpp_lib_constexpr_algorithms >= 201806L
    ::learn::ExampleChecks checks{"part2/stage07/section05/constexpr_std_vector_cpp20"};

    const auto statistics = analyze_values();
    LEARN_EXPECT_EQ(checks, statistics.sum, 15);
    LEARN_EXPECT_EQ(checks, statistics.median, 3);
    LEARN_EXPECT_EQ(checks, statistics.distinct_count, 5uz);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section05/constexpr_std_vector_cpp20",
                                               "constexpr vector and algorithms");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_std_vector_cpp20", run>;

}  // namespace
