// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : storage_duration_phases
// Topic id : part6/b/section01/storage_duration_phases
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <memory>
#include <ranges>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/b/section01/storage_duration_phases";

int namespace_value = 1;  // Static storage duration.
thread_local int per_thread = 2;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int automatic = 3;
    auto dynamic = std::make_unique<int>(4);
    LEARN_EXPECT_EQ(checks, namespace_value, 1);
    LEARN_EXPECT_EQ(checks, per_thread, 2);
    LEARN_EXPECT_EQ(checks, automatic, 3);
    LEARN_EXPECT_EQ(checks, *dynamic, 4);
    int worker_value{};
    std::jthread worker{[&] {
        per_thread = 7;
        worker_value = per_thread;
    }};
    worker.join();
    LEARN_EXPECT_EQ(checks, worker_value, 7);
    LEARN_EXPECT_EQ(checks, per_thread, 2);  // A different object exists in each thread.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section01/storage_duration_phases", run>;

}  // namespace
