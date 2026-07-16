// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_barrier_cpp20
// Topic id : part2/stage11/section02/std_barrier_cpp20
//

#include "learn/example_support.hpp"

#include <array>
#include <barrier>
#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 201907L
    learn::ExampleChecks checks{"part2/stage11/section02/std_barrier_cpp20"};
    int completed_phases = 0;
    std::barrier phase{3, [&completed_phases]() noexcept { ++completed_phases; }};
    std::array<int, 3> values{};
    std::array<std::thread, 3> workers{
        std::thread{[&] {
            values[0] = 1;
            phase.arrive_and_wait();
            values[0] *= 2;
            phase.arrive_and_wait();
        }},
        std::thread{[&] {
            values[1] = 2;
            phase.arrive_and_wait();
            values[1] *= 2;
            phase.arrive_and_wait();
        }},
        std::thread{[&] {
            values[2] = 3;
            phase.arrive_and_wait();
            values[2] *= 2;
            phase.arrive_and_wait();
        }},
    };
    for (std::thread& worker : workers) {
        worker.join();
    }

    LEARN_EXPECT_EQ(checks, completed_phases, 2);
    LEARN_EXPECT_EQ(checks, values, (std::array{2, 4, 6}));
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage11/section02/std_barrier_cpp20", "std::barrier");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_barrier_cpp20", run>;

}  // namespace
