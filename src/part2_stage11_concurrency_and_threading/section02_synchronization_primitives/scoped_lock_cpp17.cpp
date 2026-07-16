// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section02_synchronization_primitives
// Item     : scoped_lock_cpp17
// Topic id : part2/stage11/section02/scoped_lock_cpp17
//

#include "learn/example_support.hpp"

#include <mutex>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage11/section02/scoped_lock_cpp17"};
    std::mutex first_mutex;
    std::mutex second_mutex;
    int first_balance = 100;
    int second_balance = 100;

    const auto transfer = [](int& from, int& to, std::mutex& from_mutex, std::mutex& to_mutex, const int amount) {
        std::scoped_lock lock{from_mutex, to_mutex};
        from -= amount;
        to += amount;
    };

    std::thread forward{
        transfer, std::ref(first_balance), std::ref(second_balance), std::ref(first_mutex), std::ref(second_mutex), 10};
    std::thread backward{
        transfer, std::ref(second_balance), std::ref(first_balance), std::ref(second_mutex), std::ref(first_mutex), 20};
    forward.join();
    backward.join();

    LEARN_EXPECT_EQ(checks, first_balance, 110);
    LEARN_EXPECT_EQ(checks, second_balance, 90);
    LEARN_EXPECT_EQ(checks, first_balance + second_balance, 200);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section02/scoped_lock_cpp17", run>;

}  // namespace
