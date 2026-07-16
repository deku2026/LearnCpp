// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : osyncstream_cpp20
// Topic id : part2/stage11/section05/osyncstream_cpp20
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <sstream>
#include <string>
#include <string_view>
#include <syncstream>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part2/stage11/section05/osyncstream_cpp20";

[[nodiscard]] std::size_t occurrence_count(std::string_view text, std::string_view token) {
    std::size_t count{};
    std::size_t position{};
    while ((position = text.find(token, position)) != std::string_view::npos) {
        ++count;
        position += token.size();
    }
    return count;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::ostringstream destination;
    constexpr int workers = 4;
    std::array<std::jthread, workers> threads;
    for (int index = 0; index < workers; ++index) {
        threads[static_cast<std::size_t>(index)] = std::jthread{[&destination, index] {
            std::osyncstream chunk{destination};
            chunk << "[worker:" << index << "]";
            // Destruction emits the entire buffered chunk under synchronization.
        }};
    }
    for (auto& thread : threads) {
        thread.join();
    }

    const std::string output = destination.str();
    for (int index = 0; index < workers; ++index) {
        const std::string token = "[worker:" + std::to_string(index) + "]";
        LEARN_EXPECT_EQ(checks, occurrence_count(output, token), 1U);
    }
    LEARN_EXPECT_EQ(checks, output.size(), std::string{"[worker:0]"}.size() * workers);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section05/osyncstream_cpp20", run>;

}  // namespace
