// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : contains_member_cpp20
// Topic id : part2/stage10/section02/contains_member_cpp20
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/contains_member_cpp20";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::set<int> ordered{1, 3};
    const std::unordered_set<std::string> hashed{"red", "blue"};
    const std::map<int, std::string> mapped{{1, "one"}};
    LEARN_EXPECT(checks, ordered.contains(3));
    LEARN_EXPECT(checks, !hashed.contains("green"));
    LEARN_EXPECT(checks, mapped.contains(1));
    LEARN_EXPECT(checks, mapped.find(2) == mapped.end());  // Equivalent membership question, iterator result.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section02/contains_member_cpp20", run>;

}  // namespace
