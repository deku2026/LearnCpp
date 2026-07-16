// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : barrier_strengthening_cpp23
// Topic id : part2/stage11/section02/barrier_strengthening_cpp23
//

#include "learn/example_support.hpp"

#include <barrier>
#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_barrier) && __cpp_lib_barrier >= 202302L
    learn::ExampleChecks checks{"part2/stage11/section02/barrier_strengthening_cpp23"};
    int completion_value = 0;
    int worker_observed = 0;
    std::barrier phase{2, [&completion_value]() noexcept { completion_value = 42; }};

    std::thread worker([&] {
        phase.arrive_and_wait();
        worker_observed = completion_value;
    });
    phase.arrive_and_wait();
    const int main_observed = completion_value;
    worker.join();

    LEARN_EXPECT_EQ(checks, main_observed, 42);
    LEARN_EXPECT_EQ(checks, worker_observed, 42);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(
        "part2/stage11/section02/barrier_strengthening_cpp23",
        "C++23 strengthened std::barrier guarantees (__cpp_lib_barrier >= 202302L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/barrier_strengthening_cpp23", run>;

}  // namespace
