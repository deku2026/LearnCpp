// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : data_race_is_ub
// Topic id : part2/stage09/section08/data_race_is_ub
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <thread>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/data_race_is_ub";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::atomic<int> counter{0};
    std::vector<std::jthread> workers;
    for (int worker = 0; worker < 2; ++worker) {
        workers.emplace_back([&] {
            for (int iteration = 0; iteration < 1'000; ++iteration) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    workers.clear();  // jthread destruction joins before the observation.
    LEARN_EXPECT_EQ(checks, counter.load(std::memory_order_relaxed), 2'000);
#if 0
    int raced = 0;  // Concurrent unsynchronized writes to this object would be undefined.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/data_race_is_ub", run>;

}  // namespace
