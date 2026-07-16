// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : unique_lock
// Topic id : part2/stage11/section02/unique_lock
//

#include "learn/example_support.hpp"

#include <mutex>
#include <utility>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/unique_lock"};
    std::mutex mutex;
    int value = 0;

    std::unique_lock deferred{mutex, std::defer_lock};
    LEARN_EXPECT(checks, !deferred.owns_lock());
    deferred.lock();
    value = 1;
    LEARN_EXPECT(checks, deferred.owns_lock());
    deferred.unlock();
    LEARN_EXPECT(checks, !deferred.owns_lock());

    deferred.lock();
    std::unique_lock moved = std::move(deferred);
    LEARN_EXPECT(checks, !deferred.owns_lock());
    LEARN_EXPECT(checks, moved.owns_lock());
    value += 41;
    moved.unlock();

    LEARN_EXPECT_EQ(checks, value, 42);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/unique_lock", run>;

}  // namespace
