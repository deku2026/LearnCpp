// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : std_hash_load_factor
// Topic id : part2/stage10/section02/std_hash_load_factor
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section02/std_hash_load_factor";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::unordered_map<std::string, int> values;
    values.max_load_factor(0.75F);
    values.reserve(20);
    const auto buckets = values.bucket_count();
    for (int index = 0; index < 10; ++index) {
        values.emplace("key" + std::to_string(index), index);
    }
    LEARN_EXPECT(checks, values.bucket_count() >= buckets);
    LEARN_EXPECT(checks, values.load_factor() <= values.max_load_factor());
    LEARN_EXPECT_EQ(checks, std::hash<std::string>{}("same"), std::hash<std::string>{}("same"));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section02/std_hash_load_factor", run>;

}  // namespace
