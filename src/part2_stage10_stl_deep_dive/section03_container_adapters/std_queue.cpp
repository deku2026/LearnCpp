// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_queue
// Topic id : part2/stage10/section03/std_queue
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <queue>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section03/std_queue";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::queue<std::string> jobs;
    jobs.push("first");
    jobs.emplace("second");
    LEARN_EXPECT_EQ(checks, jobs.front(), std::string{"first"});
    LEARN_EXPECT_EQ(checks, jobs.back(), std::string{"second"});
    jobs.pop();
    LEARN_EXPECT_EQ(checks, jobs.front(), std::string{"second"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section03/std_queue", run>;

}  // namespace
