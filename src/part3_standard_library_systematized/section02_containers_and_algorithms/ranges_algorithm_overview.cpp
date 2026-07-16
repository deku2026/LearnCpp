// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : ranges_algorithm_overview
// Topic id : part3/section02/ranges_algorithm_overview
// Reference: C++23 working draft [algorithms.parallel.overloads], [range.alg].

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <iterator>
#include <list>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

namespace {

struct Record {
    int id;
    std::string name;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/ranges_algorithm_overview";
    learn::ExampleChecks checks{topic};

    std::vector records{Record{3, "Grace"}, Record{1, "Ada"}, Record{2, "Bjarne"}};

    // Ranges overloads take a range directly, are constrained, and support projections.
    std::ranges::sort(records, std::ranges::less{}, &Record::id);
    LEARN_EXPECT_EQ(checks, records.front().name, std::string{"Ada"});
    LEARN_EXPECT(checks, std::ranges::is_sorted(records, {}, &Record::id));
    const auto found = std::ranges::find(records, std::string{"Bjarne"}, &Record::name);
    LEARN_EXPECT(checks, found != records.end());
    LEARN_EXPECT_EQ(checks, found->id, 2);

    std::array<int, 3> ids{};
    const auto copied = std::ranges::transform(records, ids.begin(), &Record::id);
    LEARN_EXPECT(checks, copied.in == records.end());
    LEARN_EXPECT(checks, copied.out == ids.end());
    LEARN_EXPECT_EQ(checks, ids, (std::array{1, 2, 3}));

    std::vector numbers{1, 2, 2, 3, 2};
    const auto tail = std::ranges::remove(numbers, 2);
    numbers.erase(tail.begin(), tail.end());
    LEARN_EXPECT_EQ(checks, numbers, std::vector<int>({1, 3}));

    // Calls on temporary non-borrowed ranges return dangling rather than an invalid iterator.
    using TemporaryFind = decltype(std::ranges::find(std::vector{1, 2, 3}, 2));
    static_assert(std::same_as<TemporaryFind, std::ranges::dangling>);
    static_assert(std::ranges::random_access_range<decltype(records)>);
#if 0
    std::list<int> linked{3, 1, 2};
    std::ranges::sort(linked);  // Ill-formed: sort requires a random-access range.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/ranges_algorithm_overview", run>;

}  // namespace
