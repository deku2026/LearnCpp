// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : data_race_deep
// Topic id : part6/g/section03/data_race_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/data_race_deep"};

    std::atomic<int> completed{0};
    std::mutex results_mutex;
    std::vector<int> results;
    {
        std::array<std::jthread, 4> workers;
        for (int id = 0; id < static_cast<int>(workers.size()); ++id) {
            workers[static_cast<std::size_t>(id)] = std::jthread{[&, id] {
                {
                    const std::scoped_lock lock{results_mutex};
                    results.push_back(id);
                }
                completed.fetch_add(1, std::memory_order_relaxed);
            }};
        }
    }
    LEARN_EXPECT_EQ(checks, completed.load(std::memory_order_relaxed), 4);
    LEARN_EXPECT_EQ(checks, results.size(), 4U);

#if 0
    int shared = 0;
    std::jthread first{[&] { ++shared; }};
    std::jthread second{[&] { ++shared; }};  // Conflicting unsynchronized accesses.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/data_race_deep", run>;

}  // namespace
