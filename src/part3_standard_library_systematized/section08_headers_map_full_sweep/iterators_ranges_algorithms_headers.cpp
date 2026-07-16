// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : iterators_ranges_algorithms_headers
// Topic id : part3/section08/iterators_ranges_algorithms_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <execution>
#include <functional>
#include <iterator>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section08/iterators_ranges_algorithms_headers";

constexpr std::array<std::string_view, 7> headers{
    "algorithm", "execution", "functional", "iterator", "numeric", "ranges", "version",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{4, 1, 3, 2};
#if defined(__cpp_lib_parallel_algorithm) && __cpp_lib_parallel_algorithm >= 201603L
    std::sort(std::execution::seq, values.begin(), values.end());
#else
    std::sort(values.begin(), values.end());
#endif
    auto middle = values | std::views::drop(1) | std::views::take(2);
    LEARN_EXPECT(checks, std::ranges::equal(middle, std::array{2, 3}));
    LEARN_EXPECT_EQ(checks, std::accumulate(values.begin(), values.end(), 0), 10);
    LEARN_EXPECT_EQ(checks, headers.size(), 7U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/iterators_ranges_algorithms_headers", run>;

}  // namespace
