// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_call_once
// Topic id : part2/stage11/section02/std_call_once
//

#include "learn/example_support.hpp"

#include <array>
#include <mutex>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/std_call_once"};
    std::once_flag flag;
    int initialization_calls = 0;
    int initialized_value = 0;
    std::array<int, 3> observed{};

    const auto initialize = [&] {
        ++initialization_calls;
        initialized_value = 42;
    };
    std::array<std::thread, 3> workers{
        std::thread{[&] {
            std::call_once(flag, initialize);
            observed[0] = initialized_value;
        }},
        std::thread{[&] {
            std::call_once(flag, initialize);
            observed[1] = initialized_value;
        }},
        std::thread{[&] {
            std::call_once(flag, initialize);
            observed[2] = initialized_value;
        }},
    };
    for (std::thread& worker : workers) {
        worker.join();
    }

    LEARN_EXPECT_EQ(checks, initialization_calls, 1);
    LEARN_EXPECT_EQ(checks, observed, (std::array{42, 42, 42}));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_call_once", run>;

}  // namespace
