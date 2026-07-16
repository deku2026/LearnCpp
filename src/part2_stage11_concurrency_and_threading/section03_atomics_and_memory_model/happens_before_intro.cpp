// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : happens_before_intro
// Topic id : part2/stage11/section03/happens_before_intro
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/happens_before_intro";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::string result;
    std::jthread worker{[&result] {
        result = "finished";
        // The write is sequenced-before thread completion.
    }};
    worker.join();
    // Thread completion synchronizes with successful join; transitivity gives
    // a happens-before edge from the write to this read.
    LEARN_EXPECT_EQ(checks, result, std::string{"finished"});

    int value{};
    {
        std::jthread scoped_worker{[&value] { value = 42; }};
    }  // jthread destructor requests stop and joins
    LEARN_EXPECT_EQ(checks, value, 42);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/happens_before_intro", run>;

}  // namespace
