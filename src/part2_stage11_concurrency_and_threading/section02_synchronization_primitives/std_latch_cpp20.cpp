// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_latch_cpp20
// Topic id : part2/stage11/section02/std_latch_cpp20
//

#include "learn/example_support.hpp"

#include <array>
#include <latch>
#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_latch) && __cpp_lib_latch >= 201907L
    learn::ExampleChecks checks{"part2/stage11/section02/std_latch_cpp20"};
    std::latch start{1};
    std::latch finished{3};
    std::array<int, 3> values{};
    std::array<std::thread, 3> workers{
        std::thread{[&] {
            start.wait();
            values[0] = 2;
            finished.count_down();
        }},
        std::thread{[&] {
            start.wait();
            values[1] = 4;
            finished.count_down();
        }},
        std::thread{[&] {
            start.wait();
            values[2] = 6;
            finished.count_down();
        }},
    };

    start.count_down();
    finished.wait();
    for (std::thread& worker : workers) {
        worker.join();
    }
    LEARN_EXPECT_EQ(checks, values, (std::array{2, 4, 6}));
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage11/section02/std_latch_cpp20", "std::latch");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_latch_cpp20", run>;

}  // namespace
