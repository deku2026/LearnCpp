// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : iterator_categories_cpp17
// Topic id : part2/stage10/section04/iterator_categories_cpp17
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <forward_list>
#include <iterator>
#include <list>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section04/iterator_categories_cpp17";

static_assert(std::is_base_of_v<std::random_access_iterator_tag,
                                typename std::iterator_traits<std::vector<int>::iterator>::iterator_category>);
static_assert(std::is_same_v<typename std::iterator_traits<std::list<int>::iterator>::iterator_category,
                             std::bidirectional_iterator_tag>);
static_assert(std::is_same_v<typename std::iterator_traits<std::forward_list<int>::iterator>::iterator_category,
                             std::forward_iterator_tag>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1, 2, 3};
    auto iterator = values.begin();
    std::advance(iterator, 2);
    LEARN_EXPECT_EQ(checks, *iterator, 3);
    LEARN_EXPECT_EQ(checks, std::distance(values.begin(), values.end()), 3);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section04/iterator_categories_cpp17", run>;

}  // namespace
