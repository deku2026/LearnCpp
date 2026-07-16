// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : std_generator_basic
// Topic id : part2/stage12/section05/std_generator_basic

#ifdef __has_include
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section05/std_generator_basic";

[[nodiscard]] std::vector<int> fibonacci_model(std::size_t count) {
    std::vector<int> result;
    result.reserve(count);
    int current{};
    int next{1};
    for (std::size_t index = 0; index < count; ++index) {
        result.push_back(current);
        const int following = current + next;
        current = next;
        next = following;
    }
    return result;
}

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> fibonacci(std::size_t count, int* produced) {
    int current{};
    int next{1};
    for (std::size_t index = 0; index < count; ++index) {
        ++*produced;
        co_yield current;
        const int following = current + next;
        current = next;
        next = following;
    }
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};
    const std::vector expected{0, 1, 1, 2, 3, 5, 8, 13};
    LEARN_EXPECT_EQ(checks, fibonacci_model(expected.size()), expected);

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    int produced{};
    auto sequence = fibonacci(expected.size(), &produced);
    LEARN_EXPECT_EQ(checks, produced, 0);  // The coroutine is lazy until iteration begins.

    std::vector<int> actual;
    for (const int value : sequence) {
        actual.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, actual, expected);
    LEARN_EXPECT_EQ(checks, produced, static_cast<int>(expected.size()));
    static_assert(std::is_move_constructible_v<std::generator<int>>);
    static_assert(!std::is_copy_constructible_v<std::generator<int>>);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_generator >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/std_generator_basic", run>;

}  // namespace
