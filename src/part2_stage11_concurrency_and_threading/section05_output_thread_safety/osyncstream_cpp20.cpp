// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section05_output_thread_safety
// Item     : osyncstream_cpp20
// Topic id : part2/stage11/section05/osyncstream_cpp20
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <array>
#include <mutex>
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
#if !defined(__cpp_lib_syncbuf) || __cpp_lib_syncbuf < 201803L
    // Declare the mutex before the threads so stack unwinding always joins
    // every successfully created worker before destroying its lock.
    std::mutex destination_mutex;
#endif
    std::array<std::jthread, workers> threads;
#if defined(__cpp_lib_syncbuf) && __cpp_lib_syncbuf >= 201803L
    for (int index = 0; index < workers; ++index) {
        threads[static_cast<std::size_t>(index)] = std::jthread{[&destination, index] {
            std::osyncstream chunk{destination};
            chunk << "[worker:" << index << "]";
            // Destruction emits the entire buffered chunk under synchronization.
        }};
    }
#else
    for (int index = 0; index < workers; ++index) {
        threads[static_cast<std::size_t>(index)] = std::jthread{[&destination, &destination_mutex, index] {
            std::ostringstream chunk;
            chunk << "[worker:" << index << "]";
            const std::string complete_chunk = chunk.str();
            const std::scoped_lock lock{destination_mutex};
            destination << complete_chunk;
        }};
    }
#endif
    for (auto& thread : threads) {
        thread.join();
    }

    const std::string output = destination.str();
    for (int index = 0; index < workers; ++index) {
        const std::string token = "[worker:" + std::to_string(index) + "]";
        LEARN_EXPECT_EQ(checks, occurrence_count(output, token), 1U);
    }
    LEARN_EXPECT_EQ(checks, output.size(), std::string{"[worker:0]"}.size() * workers);
#if !defined(__cpp_lib_syncbuf) || __cpp_lib_syncbuf < 201803L
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return ::learn::ExampleChecks::unavailable(kTopic, "__cpp_lib_syncbuf >= 201803L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section05/osyncstream_cpp20", run>;

}  // namespace
