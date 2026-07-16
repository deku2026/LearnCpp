// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_counting_semaphore_cpp20
// Topic id : part2/stage11/section02/std_counting_semaphore_cpp20
//

#include "learn/example_support.hpp"

#include <array>
#include <semaphore>
#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    learn::ExampleChecks checks{"part2/stage11/section02/std_counting_semaphore_cpp20"};
    std::counting_semaphore<2> permits{0};
    int payload = 42;
    std::array<int, 2> observed{};
    std::thread first([&] {
        permits.acquire();
        observed[0] = payload;
    });
    std::thread second([&] {
        permits.acquire();
        observed[1] = payload;
    });

    permits.release(2);
    first.join();
    second.join();
    LEARN_EXPECT_EQ(checks, observed, (std::array{42, 42}));
    LEARN_EXPECT(checks, !permits.try_acquire());
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage11/section02/std_counting_semaphore_cpp20",
                                             "std::counting_semaphore");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_counting_semaphore_cpp20", run>;

}  // namespace
