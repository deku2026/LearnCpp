// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : move_should_be_noexcept_for_vector_growth
// Topic id : part2/stage09/section02/move_should_be_noexcept_for_vector_growth
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section02/move_should_be_noexcept_for_vector_growth";

template <bool NoThrowMove>
struct Tracked {
    explicit Tracked(int input) : value(input) {}
    Tracked(const Tracked& other) : value(other.value) { ++copies; }
    Tracked(Tracked&& other) noexcept(NoThrowMove) : value(std::exchange(other.value, 0)) { ++moves; }

    int value;
    inline static int copies{};
    inline static int moves{};
};

template <class T>
std::pair<int, int> grow() {
    T::copies = 0;
    T::moves = 0;
    std::vector<T> values;
    values.reserve(1);
    values.emplace_back(1);
    values.emplace_back(2);  // Reallocation uses move_if_noexcept-like selection.
    return {T::copies, T::moves};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto stable = grow<Tracked<true>>();
    const auto risky = grow<Tracked<false>>();
    LEARN_EXPECT_EQ(checks, stable.first, 0);
    LEARN_EXPECT(checks, stable.second >= 1);
    LEARN_EXPECT(checks, risky.first >= 1);  // A usable copy protects vector's strong guarantee.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage09/section02/move_should_be_noexcept_for_vector_growth", run>;

}  // namespace
