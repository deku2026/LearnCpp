// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section01_threads_and_tasks
// Item     : std_jthread_cpp20
// Topic id : part2/stage11/section01/std_jthread_cpp20
//

#include "learn/example_support.hpp"

#include <thread>
#include <version>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_jthread) && __cpp_lib_jthread >= 201911L
    learn::ExampleChecks checks{"part2/stage11/section01/std_jthread_cpp20"};
    int result = 0;
    {
        std::jthread worker([&result] { result = 21 * 2; });
        LEARN_EXPECT(checks, worker.joinable());
    }  // Destructor requests stop and joins; reading result is safe after this point.

    LEARN_EXPECT_EQ(checks, result, 42);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage11/section01/std_jthread_cpp20", "std::jthread");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section01/std_jthread_cpp20", run>;

}  // namespace
