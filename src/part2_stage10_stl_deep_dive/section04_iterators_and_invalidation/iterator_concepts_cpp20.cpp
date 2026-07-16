// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : iterator_concepts_cpp20
// Topic id : part2/stage10/section04/iterator_concepts_cpp20
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <forward_list>
#include <iterator>
#include <list>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section04/iterator_concepts_cpp20";

static_assert(std::contiguous_iterator<std::vector<int>::iterator>);
static_assert(std::bidirectional_iterator<std::list<int>::iterator>);
static_assert(std::forward_iterator<std::forward_list<int>::iterator>);
static_assert(!std::bidirectional_iterator<std::forward_list<int>::iterator>);

template <std::random_access_iterator Iterator>
auto middle(Iterator first, Iterator last) {
    return first + (last - first) / 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1, 2, 3, 4, 5};
    LEARN_EXPECT_EQ(checks, *middle(values.begin(), values.end()), 3);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section04/iterator_concepts_cpp20", run>;

}  // namespace
