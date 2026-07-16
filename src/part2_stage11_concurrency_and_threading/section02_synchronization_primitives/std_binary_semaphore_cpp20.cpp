// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_binary_semaphore_cpp20
// Topic id : part2/stage11/section02/std_binary_semaphore_cpp20
//

#include "learn/example_support.hpp"

#include <semaphore>
#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_semaphore) && __cpp_lib_semaphore >= 201907L
    learn::ExampleChecks checks{"part2/stage11/section02/std_binary_semaphore_cpp20"};
    std::binary_semaphore ready{0};
    std::binary_semaphore acknowledged{0};
    int payload = 0;
    int observed = 0;

    std::thread worker([&] {
        ready.acquire();
        observed = payload;
        acknowledged.release();
    });
    payload = 42;
    ready.release();
    acknowledged.acquire();
    worker.join();

    LEARN_EXPECT_EQ(checks, observed, 42);
    LEARN_EXPECT(checks, !ready.try_acquire());
    LEARN_EXPECT(checks, !acknowledged.try_acquire());
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage11/section02/std_binary_semaphore_cpp20",
                                             "std::binary_semaphore");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_binary_semaphore_cpp20", run>;

}  // namespace
