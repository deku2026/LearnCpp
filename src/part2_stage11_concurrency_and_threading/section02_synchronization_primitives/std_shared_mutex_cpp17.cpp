// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : std_shared_mutex_cpp17
// Topic id : part2/stage11/section02/std_shared_mutex_cpp17
//

#include "learn/example_support.hpp"

#include <array>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/std_shared_mutex_cpp17"};
    std::shared_mutex mutex;
    int configuration = 0;
    std::promise<void> published;
    const std::shared_future<void> ready = published.get_future().share();
    std::array<int, 2> observed{};

    std::thread writer([&] {
        {
            std::unique_lock lock{mutex};
            configuration = 42;
        }
        published.set_value();
    });
    std::thread first_reader([&] {
        ready.wait();
        std::shared_lock lock{mutex};
        observed[0] = configuration;
    });
    std::thread second_reader([&] {
        ready.wait();
        std::shared_lock lock{mutex};
        observed[1] = configuration;
    });

    writer.join();
    first_reader.join();
    second_reader.join();
    LEARN_EXPECT_EQ(checks, observed, (std::array{42, 42}));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/std_shared_mutex_cpp17", run>;

}  // namespace
